package com.thelogicmaster.switchgdx;

import com.badlogic.gdx.audio.AudioDevice;

public class SwitchAudioDevice implements AudioDevice {

    private final int rate;
    private final boolean mono;
    private long handle;
    
    public SwitchAudioDevice(int rate, boolean mono) {
        this.rate = rate;
        this.mono = mono;
        handle = create(rate, mono);
    }
    
    @Override
    public boolean isMono() {
        return mono;
    }

    @Override
    public void writeSamples(short[] shorts, int index, int length) {
        float[] samples = new float[length];
        for (int i = 0; i < length; i++)
            samples[i] = shorts[index + i] / (float)Short.MAX_VALUE;
        sample(samples, 0, length);
    }

    @Override
    public void writeSamples(float[] floats, int index, int length) {
        sample(floats, index, length);
    }

    @Override
    public int getLatency() {
        return 0;
    }

    @Override
    public void setVolume(float v) {
    }

    @Override
    public void pause() {
    }

    @Override
    public void resume() {
    }

    @Override
    public void dispose() {
        if (handle == 0) return;
        dispose(handle);
        handle = 0;
    }

    @Override
    protected void finalize() {
        dispose();
    }

    private static native long create(int rate, boolean mono);
    
    private static native void sample(float[] samples, int index, int length);
    
    private static native void dispose(long handle);
}
