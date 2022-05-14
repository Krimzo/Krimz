import engine.*;
import engine.math.*;
import engine.script.*;
import engine.input.*;
import engine.entity.*;


public class FakeHit extends Entity implements Script {

	private Float3 startPosition = new Float3();
	private float startVelocity = 0.0f;
	private boolean shouldChange() {
		if ((this.position.sub(startPosition).len() + this.collider.scale.x) >= startVelocity) {
			return true;
		}
		return false;
	}

	// Called on first frame
	public void start() {
		startPosition = new Float3(this.position);
		startVelocity = this.velocity.len();
	}

	// Called every frame
	public void update() {
		if (shouldChange()) {
			this.material.colorMap = "green";
		}
	}
}
