import engine.*;

public class Test extends Entity implements Script {

    public void start() {

    }

    public void update() {
        final float radius = 15.0f;
        final float angle = elapsedT * 30.0f + (this.name.equals("Horse1") ? 90.0f : -90.0f);

        this.position.x = radius * (float)Math.sin(Math.toRadians(angle));
        this.position.z = radius * (float)Math.cos(Math.toRadians(angle));
        this.rotation.y = angle - 90.0f;
    }
}
