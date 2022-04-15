import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;


public class Grow extends Entity implements Script {

    // Called on first frame
	public void start() {

	}

	// Called every frame
    public void update() {
        this.scale = new Float3(Time.elapsedT * 0.1f);
    }
}
