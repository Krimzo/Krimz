import engine.*;

public class Test extends Entity implements Script {

    public void start() {

    }

    public void update() {
        final float r = 15;
        final float angle = elapsedT * 30.0f - 90.0f;
        this.position.x = r * (float)Math.sin(Math.toRadians(angle));
        this.position.z = r * (float)Math.cos(Math.toRadians(angle));
    }
}
