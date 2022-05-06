import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;
import engine.entity.*;


public class Spin extends Entity implements Script {

	public static int classCounter = 0;

	private int id = 0;
	private float origRot = 0.0f;

	public Spin() {
		id = classCounter++ % 3;
	}

	// Called on first frame
	public void start() {
		if (id == 0) {
			origRot = this.rotation.x;
		}
		else if (id == 1) {
			origRot = this.rotation.y;
		}
		else if (id == 2) {
			origRot = this.rotation.z;
		}
	}

	// Called every frame
	public void update() {
		final float rotVal = (float)Math.sin(Time.elapsedT) * 45.0f;
		if (id == 0) {
			this.rotation.x = origRot + rotVal;
		}
		else if (id == 1) {
			this.rotation.y = origRot + rotVal;
		}
		else if (id == 2) {
			this.rotation.z = origRot + rotVal;
		}
	}
}