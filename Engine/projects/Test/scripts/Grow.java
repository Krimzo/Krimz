import engine.*;
import engine.math.*;
import engine.script.*;


public class Grow extends Entity implements Script {

    // Called on first frame
	public void start() {

	}

	// Called every frame
    public void update() {
        this.scale = new Float3(Engine.elapsedT * 0.1f);
    }
}
