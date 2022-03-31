import engine.*;

public class Test extends Entity implements Script {

    public void start() {

    }

    public void update() {
        this.size = new Float3(Engine.elapsedT * 0.1f);
    }
}
