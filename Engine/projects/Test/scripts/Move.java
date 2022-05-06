import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;
import engine.entity.*;


public class Move extends Entity implements Script {

	// Called on first frame
	public void start() {

	}

	// Called every frame
	public void update() {
		this.position.x = (float)Math.sin(Time.elapsedT) * 5.55f;
	}
}
