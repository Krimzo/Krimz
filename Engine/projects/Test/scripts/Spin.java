import engine.*;
import engine.math.*;
import engine.script.*;


public class Spin extends Entity implements Script {

	// Called on first frame
	public void start() {

	}

	// Called every frame
	public void update() {
		this.rotation.y = (float)Math.sin(Engine.elapsedT) * 45.0f;
	}
}
