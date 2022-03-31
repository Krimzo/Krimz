import engine.*;

public class Spinner extends Entity implements Script {

    public void start() {
        
    }

    public void update() {
        this.rotation.y = (float)Math.sin(Engine.elapsedT) * 90.0f;
    }
}
