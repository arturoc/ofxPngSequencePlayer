/*
 * ofxPngSequencePlayer.cpp
 *
 *  Created on: 04/10/2011
 *      Author: arturo
 */

#include "ofxPngSequencePlayer.h"
#include "ofMath.h"
#include "ofImage.h"
#include "ofFmodSoundPlayer.h"

ofxPngSequencePlayer::ofxPngSequencePlayer() {
	currentFrame = 0;
	bPlaying = false;
	oneFrameTime = 0;
	isNewFrame = 0;
	isNewPixels = 0;
	lastFrameTime = 0;
	speed = 1;
    drawTexture = NULL;
}

ofxPngSequencePlayer::~ofxPngSequencePlayer() {
	close();
}

void ofxPngSequencePlayer::setFps(int _fps){
	fps = _fps;
	oneFrameTime = 1.f/_fps*1000.;
}

void ofxPngSequencePlayer::setAudioTrack(string audio){
	//ofPtr<ofBaseSoundPlayer> fmodSoundPlayer = ofPtr<ofBaseSoundPlayer>(new ofFmodSoundPlayer);
	//soundPlayer.setPlayer(fmodSoundPlayer);
	//soundPlayer.loadSound(audio,true);
}

bool ofxPngSequencePlayer::loadMovie(string _folder){
	folder.allowExt("png");
	folder.listDir(_folder);
	if(!folder.exists()){
		ofLogError("ofxPngSequencePlayer") << "Error couldn't load movie, folder doesn't exist";
		return false;
	}

	if(folder.size()==0){
		ofLogError("ofxPngSequencePlayer") << "Error couldn't load movie, 0 files found";
		return false;
	}

	currentFrame = 0;
	folder.sort();
	frames.push(ofPixels());
	ofLoadImage(frames.back(),folder.getPath(currentFrame));
	pixels = frames.back();

    
    if(drawTexture == NULL) drawTexture = new ofTexture();
    if (drawTexture->bAllocated()) drawTexture->clear();
    drawTexture->allocate(70, 70, GL_RGBA);
    drawTexture->loadData(getPixels(),70, 70, GL_RGBA);
	return true;
}

void ofxPngSequencePlayer::close(){
	stop();
	folder.close();
    drawTexture->clear();
	//soundPlayer.unloadSound();
}


void ofxPngSequencePlayer::draw(int x, int y, float width, float height)
{
    // TODO: Optimizar el dibujo del hotspot.
    drawTexture->loadData(getPixels(),70, 70, GL_RGBA);
    //img.setFromPixels(getPixels(), 70, 70, OF_IMAGE_COLOR_ALPHA);
    drawTexture->draw(x,y,width,height);
}


void ofxPngSequencePlayer::play()
{
	currentFrame = 0;
	lastFrameTime = ofGetElapsedTimeMillis();
	startThread(true,false);
	//soundPlayer.play();
	bPlaying = true;
}

void ofxPngSequencePlayer::stop(){
	currentFrame = 0;
	//soundPlayer.stop();
	waitForThread(true);
	pixels.set(0);
	while(!frames.empty()) frames.pop();
	bPlaying = false;
	isNewFrame = true;
}

void ofxPngSequencePlayer::update(){
	if(isNewPixels){
		isNewFrame = true;
		isNewPixels = false;
	}else{
		isNewFrame = false;
	}
}

bool ofxPngSequencePlayer::isFrameNew(){
	return isNewFrame;
}

void ofxPngSequencePlayer::threadedFunction(){
	while(isThreadRunning()){
        if(frames.size()<10 && currentFrame+frames.size()+1<getTotalNumFrames()){
			frames.push(ofPixels());
			ofLoadImage(frames.back(),folder.getPath(currentFrame+frames.size()));
		}

		if(frames.empty()) return;
		int currentTime = ofGetElapsedTimeMillis();
		int diff = currentTime - lastFrameTime;
		if(diff>=oneFrameTime){
            
			lastFrameTime = currentTime - (diff-oneFrameTime*(diff/oneFrameTime));
            
			for(int i=0;i<(diff/oneFrameTime-1) && !frames.empty();i++){
            
                if(frames.size()==1){
					pixels = frames.front();
				}
				frames.pop();
				//currentFrame = ofClamp(++currentFrame,0,(folder.size()-1)/2);
                currentFrame += 1;
                if(currentFrame+1 >= getTotalNumFrames()) currentFrame = 0;
			}
            
			if(!frames.empty()){
				pixels = frames.front();
				frames.pop();
			}
        
            //currentFrame = ofClamp(++currentFrame,0,(folder.size()-1)/2);
			currentFrame += 1;
            if(currentFrame+1 >= getTotalNumFrames()) currentFrame = 0;
            isNewPixels = true;
             
		}
	}
}

unsigned char * ofxPngSequencePlayer::getPixels(){
	return pixels.getPixels();
}

ofPixels & ofxPngSequencePlayer::getPixelsRef(){
	return pixels;
}

float ofxPngSequencePlayer::getWidth(){
	return pixels.getWidth();
}

float ofxPngSequencePlayer::getHeight(){
	return pixels.getHeight();
}

bool ofxPngSequencePlayer::isPaused(){
	return isThreadRunning();
}

bool ofxPngSequencePlayer::isLoaded(){
	return pixels.isAllocated();
}

bool ofxPngSequencePlayer::isPlaying(){
	return bPlaying;
}

float ofxPngSequencePlayer::getPosition(){
	return float(currentFrame) / float(folder.size());
}

float ofxPngSequencePlayer::getSpeed(){
	return speed;
}

float ofxPngSequencePlayer::getDuration(){
	return oneFrameTime * folder.size();
}

bool ofxPngSequencePlayer::getIsMovieDone(){
	return currentFrame == folder.size()-1;
}

void ofxPngSequencePlayer::setPaused(bool bPause){
	if(bPause){
		stopThread();
		//soundPlayer.setPaused(true);
	}else if(!isPlaying()){
		lastFrameTime = ofGetElapsedTimeMillis();
		startThread(true,false);
		//soundPlayer.setPaused(false);
	}
}

void ofxPngSequencePlayer::setPosition(float pct){
	pct = ofClamp(pct,0,1);
	lock();
	currentFrame = pct * folder.size();
	lastFrameTime = ofGetElapsedTimeMillis();
	while(!frames.empty()) frames.pop();
	unlock();
}

void ofxPngSequencePlayer::setLoopState(ofLoopType state){
    loopState = state;
}

void ofxPngSequencePlayer::setSpeed(float _speed){
	speed = _speed;
}
void ofxPngSequencePlayer::setFrame(int frame){
	lock();
	currentFrame = ofClamp(frame,0,folder.size()) - 1;
	lastFrameTime = ofGetElapsedTimeMillis();
	while(!frames.empty()) frames.pop();
	unlock();
}

int	ofxPngSequencePlayer::getCurrentFrame(){
	return currentFrame;
}

int	ofxPngSequencePlayer::getTotalNumFrames(){
	return folder.size();
}

ofLoopType ofxPngSequencePlayer::getLoopState(){
    return loopState;
}

void ofxPngSequencePlayer::firstFrame(){
	setFrame(0);
}

void ofxPngSequencePlayer::nextFrame(){
	setFrame(currentFrame+1);
}

void ofxPngSequencePlayer::previousFrame(){
	setFrame(currentFrame-1);
}
