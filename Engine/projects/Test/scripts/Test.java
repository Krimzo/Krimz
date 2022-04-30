import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;


public class Test extends Entity implements Script {

	// Called on first frame
	public void start() {
		this.scale = new Float3(3.0f);
		this.mesh = "table";
		this.texture = "cyan";
		this.dynamic = true;
		this.gravity = false;
		this.collider.shape = Collider.Shape.MESH;
	}

	// Called every frame
	public void update() {

	}
}
