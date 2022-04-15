import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;


public class Flip extends Entity implements Script {

	// Called on first frame
	public void start() {
		this.rotation.z = 180.0f;
	}

	// Called every frame
	public void update() {

	}
}
