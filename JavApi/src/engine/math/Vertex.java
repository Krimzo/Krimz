package engine.math;

public class Vertex {
    public Float4 world;
    public Float3 texture;
    public Float3 normal;

    public Vertex() {
        world = new Float4();
        texture = new Float3();
        normal = new Float3();
    }
    public Vertex(Float4 world) {
        this.world = world;
        texture = new Float3();
        normal = new Float3();
    }
    public Vertex(Float4 world, Float3 texture) {
        this.world = world;
        this.texture = texture;
        normal = new Float3();
    }
    public Vertex(Float4 world, Float3 texture, Float3 normal) {
        this.world = world;
        this.texture = texture;
        this.normal = normal;
    }

    // Perspective division by w
    public void divByW() {
        // Getting the w reciprocal
        final float wRec = 1.0f / world.w;

        // Dividing the world data
        world = world.mul(wRec);

        // Dividing the texture data
        texture = texture.mul(wRec);
        texture.z = wRec;
    }
}
