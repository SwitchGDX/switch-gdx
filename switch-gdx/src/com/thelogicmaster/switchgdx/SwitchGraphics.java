package com.thelogicmaster.switchgdx;

import com.badlogic.gdx.AbstractGraphics;
import com.badlogic.gdx.Application;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.*;
import com.badlogic.gdx.graphics.glutils.GLVersion;

public class SwitchGraphics extends AbstractGraphics {

	private final BufferFormat bufferFormat;
	private final GLVersion glVersion;
	private final String extensions;

	private float time;
	private float deltaTime;
	private long frameId = -1;
	private int frames;
	private long previousTime;
	private int fps;

	public SwitchGraphics () {
		Gdx.gl = Gdx.gl20 = new SwitchGL();

		String versionString = Gdx.gl.glGetString(GL20.GL_VERSION);
		String vendorString = Gdx.gl.glGetString(GL20.GL_VENDOR);
		String rendererString = Gdx.gl.glGetString(GL20.GL_RENDERER);
		glVersion = new GLVersion(Application.ApplicationType.Desktop, versionString, vendorString, rendererString);

		extensions = Gdx.gl.glGetString(GL20.GL_EXTENSIONS);

		bufferFormat = new BufferFormat(8, 8, 8, 8, 24, 8, 0, false);

		previousTime = System.currentTimeMillis();
	}

	void update() {
		long timestamp = System.currentTimeMillis();
		deltaTime = (Math.max(timestamp - previousTime, 1)) / 1000.f;
		previousTime = timestamp;
		time += deltaTime;
		frameId++;
		frames++;
		if (time > 1) {
			fps = frames;
			time = 0;
			frames = 0;
		}
	}

	@Override
	public boolean isGL30Available () {
		return false;
	}

	@Override
	public boolean isGL31Available() {
		return false;
	}

	@Override
	public boolean isGL32Available() {
		return false;
	}

	@Override
	public GL20 getGL20 () {
		return Gdx.gl20;
	}

	@Override
	public GL30 getGL30 () {
		return null;
	}

	@Override
	public GL31 getGL31() {
		return null;
	}

	@Override
	public GL32 getGL32() {
		return null;
	}

	@Override
	public void setGL20 (GL20 gl20) {
		Gdx.gl = gl20;
		Gdx.gl20 = gl20;
	}

	@Override
	public void setGL30 (GL30 gl30) {

	}

	@Override
	public void setGL31(GL31 gl31) {

	}

	@Override
	public void setGL32(GL32 gl32) {

	}

	@Override
	public native int getWidth ();

	@Override
	public native int getHeight ();

	@Override
	public int getBackBufferWidth () {
		return getWidth();
	}

	@Override
	public int getBackBufferHeight () {
		return getHeight();
	}

	@Override
	public int getSafeInsetLeft () {
		return 0;
	}

	@Override
	public int getSafeInsetTop () {
		return 0;
	}

	@Override
	public int getSafeInsetBottom () {
		return 0;
	}

	@Override
	public int getSafeInsetRight () {
		return 0;
	}

	@Override
	public long getFrameId () {
		return frameId;
	}

	@Override
	public float getDeltaTime () {
		return deltaTime;
	}

	@Override
	public int getFramesPerSecond () {
		return fps;
	}

	@Override
	public GraphicsType getType () {
		return GraphicsType.LWJGL;
	}

	@Override
	public GLVersion getGLVersion () {
		return glVersion;
	}

	@Override
	public float getPpiX () {
		return 160;
	}

	@Override
	public float getPpiY () {
		return 160;
	}

	@Override
	public float getPpcX () {
		return 63;
	}

	@Override
	public float getPpcY () {
		return 63;
	}

	@Override
	public boolean supportsDisplayModeChange () {
		return false;
	}

	@Override
	public Monitor getPrimaryMonitor () {
		return getMonitor();
	}

	@Override
	public Monitor getMonitor () {
		return new SwitchMonitor(0, 0, "Switch Display");
	}

	@Override
	public Monitor[] getMonitors () {
		return new Monitor[]{getMonitor()};
	}

	@Override
	public DisplayMode[] getDisplayModes () {
		return new DisplayMode[]{getDisplayMode()};
	}

	@Override
	public DisplayMode[] getDisplayModes (Monitor monitor) {
		return new DisplayMode[]{getDisplayMode()};
	}

	@Override
	public DisplayMode getDisplayMode () {
		return new SwitchDisplayMode(getWidth(), getHeight(), 60, 32);
	}

	@Override
	public DisplayMode getDisplayMode (Monitor monitor) {
		return getDisplayMode();
	}

	@Override
	public boolean setFullscreenMode (DisplayMode displayMode) {
		return false;
	}

	@Override
	public boolean setWindowedMode (int width, int height) {
		return false;
	}

	@Override
	public void setTitle (String title) {

	}

	@Override
	public void setUndecorated (boolean undecorated) {

	}

	@Override
	public void setResizable (boolean resizable) {

	}

	@Override
	public void setVSync (boolean vsync) {

	}

	@Override
	public void setForegroundFPS (int fps) {

	}

	@Override
	public BufferFormat getBufferFormat () {
		return bufferFormat;
	}

	@Override
	public boolean supportsExtension (String extension) {
		return extensions.contains(extension);
	}

	@Override
	public void setContinuousRendering (boolean isContinuous) {

	}

	@Override
	public boolean isContinuousRendering () {
		return true;
	}

	@Override
	public void requestRendering () {

	}

	@Override
	public boolean isFullscreen () {
		return true;
	}

	@Override
	public Cursor newCursor (Pixmap pixmap, int xHotspot, int yHotspot) {
		return null;
	}

	@Override
	public void setCursor (Cursor cursor) {

	}

	@Override
	public void setSystemCursor (Cursor.SystemCursor systemCursor) {

	}

	static class SwitchDisplayMode extends DisplayMode {
		public SwitchDisplayMode (int width, int height, int refreshRate, int bitsPerPixel) {
			super(width, height, refreshRate, bitsPerPixel);
		}
	}

	static class SwitchMonitor extends Monitor {
		protected SwitchMonitor (int virtualX, int virtualY, String name) {
			super(virtualX, virtualY, name);
		}
	}
}
