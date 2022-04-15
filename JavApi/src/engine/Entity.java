package engine;

import engine.math.*;

public abstract class Entity {
    // Name
    public String name = "";

    // View
    public boolean visible = true;
    public boolean shadows = true;
    public float roughness = 0.5f;

    // Geometry
    public Float3 scale = new Float3(1.0f);
    public Float3 rotation = new Float3(0.0f);
    public Float3 position = new Float3(0.0f);

    // Physics
    public boolean dynamic = false;
    public boolean collisions = false;
    public Float3 velocity = new Float3(0.0f);
    public Float3 angular = new Float3(0.0f);
}
