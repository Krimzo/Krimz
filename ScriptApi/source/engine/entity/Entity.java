package engine.entity;

import engine.math.*;

public abstract class Entity {
    // Name
    public String name = "";

    // View
    public boolean visible = true;
    public boolean shadows = true;
    public Material material = new Material();

    // Geometry
    public Float3 scale = new Float3(1.0f);
    public Float3 rotation = new Float3(0.0f);
    public Float3 position = new Float3(0.0f);
    public String mesh = "cube";

    // Physics
    public boolean dynamic = false;
    public boolean gravity = true;
    public float friction = 0.5f;
    public float mass = 10.0f;
    public Float3 velocity = new Float3(0.0f);
    public Float3 angular = new Float3(0.0f);
    public Collider collider = new Collider();
}
