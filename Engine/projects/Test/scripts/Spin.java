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

	}

	// Called every frame
	public void update() {
		if (id == 0) {
			this.rotation.x = (float)Math.sin(Engine.elapsedT) * 45.0f;
		}
		else if (id == 1) {
			this.rotation.y = (float)Math.sin(Engine.elapsedT) * 45.0f;
		}
		else if (id == 2) {
			this.rotation.z = (float)Math.sin(Engine.elapsedT) * 45.0f;
		}
	}
}