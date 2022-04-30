package com.thelogicmaster.switchgdx;

import com.badlogic.gdx.controllers.ControllerMapping;

public class SwitchControllerMapping extends ControllerMapping {

	private static SwitchControllerMapping instance;

	private SwitchControllerMapping () {
		super(0, 1, 2, 3, 0, 1, 2, 3, 11, 10, 6, 8, 7, 9, 4, 5, 13, 15, 12, 14);
	}

	static SwitchControllerMapping getInstance() {
		if (instance == null)
			instance = new SwitchControllerMapping();
		return instance;
	}
}
