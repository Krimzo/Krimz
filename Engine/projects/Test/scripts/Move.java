import engine.*;
import engine.math.*;
import engine.script.*;


public class Move extends Entity implements Script {

	// Called on first frame
	public void start() {

	}

	// Called every frame
	public void update() {
		this.position.x = (float)Math.sin(Engine.elapsedT) * 5.55f;
	}
}
