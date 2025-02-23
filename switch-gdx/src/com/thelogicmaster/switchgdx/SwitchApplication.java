package com.thelogicmaster.switchgdx;

import com.badlogic.gdx.Application;
import com.badlogic.gdx.ApplicationListener;
import com.badlogic.gdx.ApplicationLogger;
import com.badlogic.gdx.Audio;
import com.badlogic.gdx.Files;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Graphics;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.LifecycleListener;
import com.badlogic.gdx.Net;
import com.badlogic.gdx.Preferences;
import com.badlogic.gdx.controllers.Controllers;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.Clipboard;
import com.badlogic.gdx.utils.ObjectMap;
import com.badlogic.gdx.utils.Timer;

public class SwitchApplication implements Application {

	private SwitchGraphics graphics;
	private SwitchFiles files;
	private SwitchInput input;
	private SwitchNet net;
	private SwitchAudio audio;
	
	private final ApplicationType appType;

	private boolean running;
	private ApplicationLogger applicationLogger;
	private int logLevel = LOG_INFO;
	private final ObjectMap<String, Preferences> preferences = new ObjectMap<>();
	private final ApplicationListener listener;
	private final Array<LifecycleListener> lifecycleListeners = new Array<>();
	private final Array<Runnable> runnables = new Array<>();
	private final Array<Runnable> executedRunnables = new Array<>();

	public static final boolean isSwitch = System.getProperty("os.name").equals("horizon");
	public static final boolean isUWP = System.getProperty("os.name").equals("uwp");

	public SwitchApplication(ApplicationListener listener) {
		this(listener, new Config());
	}

	public SwitchApplication (ApplicationListener listener, Config config) {
		init(config.vsync);

		this.listener = listener;
		
		appType = config.appType;

		Gdx.app = this;

		try {
			applicationLogger = new SwitchLogger();
			graphics = new SwitchGraphics();
			files = new SwitchFiles();
			input = new SwitchInput();
			net = new SwitchNet();
			audio = new SwitchAudio();

			Gdx.input = this.getInput();
			Gdx.audio = this.getAudio();
			Gdx.files = this.getFiles();
			Gdx.graphics = this.getGraphics();
			Gdx.net = this.getNet();

			new SwitchFileHandle("", Files.FileType.External).mkdirs();
			new SwitchFileHandle("", Files.FileType.Local).mkdirs();

			Controllers.preferredManager = "com.thelogicmaster.switchgdx.SwitchControllerManager";
			Controllers.getCurrent();
			SwitchControllerManager controllerManager = SwitchControllerManager.getInstance();

			running = true;
			listener.create();

			int width = 0, height = 0;

			while (running && update()) {
				int currentWidth = graphics.getWidth();
				int currentHeight = graphics.getHeight();
				if (currentWidth != width || currentHeight != height) {
					width = currentWidth;
					height = currentHeight;
					listener.resize(width, height);
				}

				graphics.update();
				audio.update(graphics.getDeltaTime());
				input.update();
				controllerManager.update();
				Timer.instance().update();
				executeRunnables();
				listener.render();
			}

			for (LifecycleListener lifecycleListener : new Array.ArrayIterable<>(lifecycleListeners))
				lifecycleListener.dispose();
			listener.dispose();
		} catch (Throwable t) {
			System.out.println("Uncaught Exception:");
			t.printStackTrace();
			System.err.println("Message: " + t.getMessage());
			Throwable cause = t.getCause();
			while (cause != null) {
				System.err.println("Message: " + cause.getMessage());
				cause = cause.getCause();
			}
		}

		dispose();

		System.exit(0);
	}

	private void executeRunnables () {
		synchronized (runnables) {
			for (int i = runnables.size - 1; i >= 0; i--)
				executedRunnables.add(runnables.get(i));
			runnables.clear();
		}
		if (executedRunnables.size == 0)
			return;
		do
			executedRunnables.pop().run();
		while (executedRunnables.size > 0);
	}

	@Override
	public ApplicationListener getApplicationListener () {
		return listener;
	}

	@Override
	public Graphics getGraphics () {
		return graphics;
	}

	@Override
	public Audio getAudio () {
		return audio;
	}

	@Override
	public Input getInput () {
		return input;
	}

	@Override
	public Files getFiles () {
		return files;
	}

	@Override
	public Net getNet () {
		return net;
	}

	@Override
	public void debug (String tag, String message) {
		if (logLevel >= LOG_DEBUG)
			getApplicationLogger().debug(tag, message);
	}

	@Override
	public void debug (String tag, String message, Throwable exception) {
		if (logLevel >= LOG_DEBUG)
			getApplicationLogger().debug(tag, message, exception);
	}

	@Override
	public void log (String tag, String message) {
		if (logLevel >= LOG_INFO)
			getApplicationLogger().log(tag, message);
	}

	@Override
	public void log (String tag, String message, Throwable exception) {
		if (logLevel >= LOG_INFO)
			getApplicationLogger().log(tag, message, exception);
	}

	@Override
	public void error (String tag, String message) {
		if (logLevel >= LOG_ERROR)
			getApplicationLogger().error(tag, message);
	}

	@Override
	public void error (String tag, String message, Throwable exception) {
		if (logLevel >= LOG_ERROR)
			getApplicationLogger().error(tag, message, exception);
	}

	@Override
	public void setLogLevel (int logLevel) {
		this.logLevel = logLevel;
	}

	@Override
	public int getLogLevel () {
		return logLevel;
	}

	@Override
	public void setApplicationLogger (ApplicationLogger applicationLogger) {
		this.applicationLogger = applicationLogger;
	}

	@Override
	public ApplicationLogger getApplicationLogger () {
		return applicationLogger;
	}

	@Override
	public ApplicationType getType () {
		return appType;
	}

	@Override
	public int getVersion () {
		return 0;
	}

	@Override
	public long getJavaHeap () {
		return 0;
	}

	@Override
	public long getNativeHeap () {
		return 0;
	}

	@Override
	public Preferences getPreferences (String name) {
		if (preferences.containsKey(name))
			return preferences.get(name);
		else {
			Preferences prefs = new SwitchPreferences(files.local(name + ".prefs"));
			preferences.put(name, prefs);
			return prefs;
		}
	}

	@Override
	public Clipboard getClipboard () {
		return new SwitchClipboard();
	}

	@Override
	public void postRunnable (Runnable runnable) {
		synchronized (runnables) {
			runnables.add(runnable);
		}
	}

	@Override
	public void exit () {
		postRunnable(() -> running = false);
	}

	@Override
	public void addLifecycleListener (LifecycleListener listener) {
		lifecycleListeners.add(listener);
	}

	@Override
	public void removeLifecycleListener (LifecycleListener listener) {
		lifecycleListeners.removeValue(listener, true);
	}

	private static native void init (boolean vsync);

	private static native boolean update ();

	private static native void dispose ();

	private static class SwitchClipboard implements Clipboard {
		@Override
		public boolean hasContents () {
			return false;
		}

		@Override
		public String getContents () {
			return null;
		}

		@Override
		public void setContents (String content) {

		}
	}

	public static class Config {

		private boolean vsync = true;
		
		private ApplicationType appType = ApplicationType.Desktop;

		public Config() {
		}

		public Config(boolean vsync) {
			this.vsync = vsync;
		}

		public Config(boolean vsync, ApplicationType appType) {
			this.vsync = vsync;
			this.appType = appType;
		}

		public boolean getVsync() {
			return vsync;
		}

		public void setVsync(boolean vsync) {
			this.vsync = vsync;
		}

		public ApplicationType getAppType() {
			return appType;
		}

		public void setAppType(ApplicationType appType) {
			this.appType = appType;
		}
	}
}
