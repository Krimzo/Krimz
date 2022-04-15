import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;


public class Shrink extends Entity implements Script {

	private Float3 savedScale = new Float3();
	private int lastScroll = 0;
	private float multi = 0.5f;

	// Called on first frame
	public void start() {
		savedScale = new Float3(scale);
		lastScroll = Mouse.scroll;
	}

	// Called every frame
	public void update() {
		// Scroll handle
		final int scrollDif = lastScroll - Mouse.scroll;
		lastScroll = Mouse.scroll;
		multi -= scrollDif * 0.1f;

		// size "display"
		if (name.equals("Display")) {
			scale.y = multi;
			return;
		}

		// Change
		if (Keys.u) {
			scale.x = savedScale.x * multi;
		}
		else {
			scale.x = savedScale.x;
		}
		if (Keys.i) {
			scale.y = savedScale.y * multi;
		}
		else {
			scale.y = savedScale.y;
		}
		if (Keys.o) {
			scale.z = savedScale.z * multi;
		}
		else {
			scale.z = savedScale.z;
		}
	}
}
