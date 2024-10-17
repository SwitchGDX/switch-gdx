package com.thelogicmaster.test;

import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.tests.utils.GdxTest;
import com.badlogic.gdx.utils.ScreenUtils;

public class GarbageCollectorTest extends GdxTest {

	@Override
	public void create () {

	}

	@Override
	public void render () {
		ScreenUtils.clear(Color.BLACK);

		for (int i = 0; i < 1000; i++) {
			byte[] test = new byte[100000];
		}
	}

	@Override
	public void dispose () {

	}
}
