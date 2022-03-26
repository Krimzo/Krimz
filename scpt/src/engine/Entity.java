package engine;

public abstract class Entity extends Engine {
    // Name
    public String name = "undef";

    // View
    public boolean visible = true;
    public boolean shadows = true;
    public float roughness = 0.5f;

    // Geometry
    public Float3 size = new Float3(1.0f);
    public Float3 rotation = new Float3(0.0f);
    public Float3 position = new Float3(0.0f);

    // Physics
    public boolean physics = false;
    public Float3 acceler = new Float3(0.0f);
    public Float3 velocity = new Float3(0.0f);
    public Float3 angular = new Float3(0.0f);
}
