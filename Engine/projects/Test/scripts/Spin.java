import engine.*;
import engine.math.*;
import engine.script.*;


public class Spin extends Entity implements Script {

	static int classCounter = 0;
	int id = 0;

	public Spin() {
		id = classCounter++ % 3;
	}

	// Called on first frame
	public void start() {
		Logger.log("It's spinning! ID=" + id);
	}

	// Called every frame
	public void update() {
		final float rotVal = (float)Math.sin(Engine.elapsedT) * 45.0f;
		if (id == 0) {
			this.rotation.x = rotVal;
		}
		else if (id == 1) {
			this.rotation.y = rotVal;
		}
		else if (id == 2) {
			this.rotation.z = rotVal;
		}
	}
}