package engine;

import engine.math.*;

public class Collider {
    public interface Shape {
        int NONE = 0;
        int BOX = 1;
        int SPHERE = 2;
        int CAPSULE = 3;
        int MESH = 4;
    }

    // Data
    public Float3 scale = new Float3(1.0f);
    public Float3 rotation = new Float3(0.0f);
    public Float3 position = new Float3(0.0f);
    public int shape = Shape.NONE;

    public Collider() {}
    public Collider(int shape) {
        this.shape = shape;
    }
}
