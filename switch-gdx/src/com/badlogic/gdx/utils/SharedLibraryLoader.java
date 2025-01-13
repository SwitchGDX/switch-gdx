package com.badlogic.gdx.utils;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;
import java.util.Random;
import java.util.UUID;

import com.badlogic.gdx.utils.Architecture.Bitness;
import com.badlogic.gdx.math.MathUtils;

public class SharedLibraryLoader {
	public static Os os;
	public static Architecture.Bitness bitness;
	public static Architecture architecture;
	static public boolean isWindows;
	static public boolean isLinux;
	static public boolean isMac;
	static public boolean isIos;
	static public boolean isAndroid;
	static public boolean isARM;
	static public boolean is64Bit;

	public SharedLibraryLoader () {
	}

	static String randomUUID () {
		return new UUID(MathUtils.random.nextLong(), MathUtils.random.nextLong()).toString();
	}

	public SharedLibraryLoader (String nativesJar) {

	}

	public String crc (InputStream input) {
		throw new UnsupportedOperationException();
	}

	public String mapLibraryName (String libraryName) {
		throw new UnsupportedOperationException();
	}

	public void load (String libraryName) {

	}

	public File extractFile (String sourcePath, String dirName) throws IOException {
		throw new UnsupportedOperationException();
	}

	public void extractFileTo (String sourcePath, File dir) throws IOException {
		throw new UnsupportedOperationException();
	}

	public static synchronized void setLoaded (String libraryName) {

	}

	public static synchronized boolean isLoaded (String libraryName) {
		return true;
	}

	public static void closeQuietly(Closeable c) {
		if (c != null) {
			try {
				c.close();
			} catch (Throwable var2) {
			}
		}

	}

	static {
		bitness = Bitness._32;
		architecture = Architecture.x86;
		if (System.getProperty("os.name").contains("Windows")) {
			os = Os.Windows;
		} else if (System.getProperty("os.name").contains("Linux")) {
			os = Os.Linux;
		} else if (System.getProperty("os.name").contains("Mac")) {
			os = Os.MacOsX;
		} else if (System.getProperty("os.name").equals("horizon") || System.getProperty("os.name").equals("uwp")) {
			os = Os.Android; // Just use Android for Switch
		}

		if (!System.getProperty("os.arch").startsWith("arm") && !System.getProperty("os.arch").startsWith("aarch64")) {
			if (System.getProperty("os.arch").startsWith("riscv")) {
				architecture = Architecture.RISCV;
			} else if (System.getProperty("os.arch").startsWith("loongarch")) {
				architecture = Architecture.LOONGARCH;
			}
		} else {
			architecture = Architecture.ARM;
		}

		if (!System.getProperty("os.arch").contains("64") && !System.getProperty("os.arch").startsWith("armv8")) {
			if (System.getProperty("os.arch").contains("128")) {
				bitness = Bitness._128;
			}
		} else {
			bitness = Bitness._64;
		}

		boolean isMOEiOS = System.getProperty("moe.platform.name") != null;
		String vm = System.getProperty("java.runtime.name");
		if (vm != null && vm.contains("Android Runtime")) {
			os = Os.Android;
			bitness = Bitness._32;
			architecture = Architecture.x86;
		}

		if (isMOEiOS || os != Os.Android && os != Os.Windows && os != Os.Linux && os != Os.MacOsX) {
			os = Os.IOS;
			bitness = Bitness._32;
			architecture = Architecture.x86;
		}

		isWindows = os == Os.Windows;
		isLinux = os == Os.Linux;
		isMac = os == Os.MacOsX;
		isIos = os == Os.IOS;
		isAndroid = os == Os.Android;
		isARM = architecture == Architecture.ARM;
		is64Bit = bitness == Bitness._64;
	}
}
