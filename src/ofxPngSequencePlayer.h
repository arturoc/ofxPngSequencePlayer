/*
 * PngSequencePlayer.h
 *
 *  Created on: 04/10/2011
 *      Author: arturo
 */

#ifndef PNGSEQUENCEPLAYER_H_
#define PNGSEQUENCEPLAYER_H_

#include <queue>
#include "ofConstants.h"
#include "ofThread.h"
#include "ofFileUtils.h"
#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofSoundPlayer.h"

class ofxPngSequencePlayer: public ofThread, public ofBaseVideoPlayer {
public:
	ofxPngSequencePlayer();
	virtual ~ofxPngSequencePlayer();

	void setFps(int fps);
	void setAudioTrack(string audio);
	bool loadMovie(string folder);
	void close();
	void play();
	void stop();

	void update();

	bool 				isFrameNew();
	unsigned char * 	getPixels();
	ofPixels & 			getPixelsRef();

	float 				getWidth();
	float 				getHeight();

	bool				isPaused();
	bool				isLoaded();
	bool				isPlaying();

	//should implement!
	float 				getPosition();
	float 				getSpeed();
	float 				getDuration();
	bool				getIsMovieDone();

	void 				setPaused(bool bPause);
	void 				setPosition(float pct);
	void 				setLoopState(ofLoopType state);
	void   				setSpeed(float speed);
	void				setFrame(int frame);  // frame 0 = first frame...

	int					getCurrentFrame();
	int					getTotalNumFrames();
	int					getLoopState();

	void				firstFrame();
	void				nextFrame();
	void				previousFrame();

protected:
	void threadedFunction();

	ofDirectory folder;
	int currentFrame;
	int fps, oneFrameTime;
	bool bPlaying;

	queue<ofPixels> frames;
	ofPixels pixels;

	int lastFrameTime;
	bool isNewFrame, isNewPixels;
	int speed;

	ofSoundPlayer soundPlayer;
};

#endif /* PNGSEQUENCEPLAYER_H_ */
