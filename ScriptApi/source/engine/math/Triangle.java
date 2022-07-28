package engine.math;

public class Triangle {
    public Vertex a;
    public Vertex b;
    public Vertex c;

    public Triangle(Vertex A, Vertex B, Vertex C) {
        a = A;
        b = B;
        c = C;
    }
    public Triangle(Triangle t) {
        a = new Vertex(t.a);
        b = new Vertex(t.b);
        c = new Vertex(t.c);
    }

    // Interpolation constant
    public Float4 interConsts() {
        final float calcConst = 1 / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
        return new Float4(
            (b.world.y - c.world.y) * calcConst,
            (c.world.x - b.world.x) * calcConst,
            (c.world.y - a.world.y) * calcConst,
            (a.world.x - c.world.x) * calcConst
        );
    }

    // Interpolation weights
    public Float3 interWeights(Float4 interConst, Float2 pos) {
        final float dx = pos.x - c.world.x;
        final float dy = pos.y - c.world.y;
        final float interWeight1 = dx * interConst.x + dy * interConst.y;
        final float interWeight2 = dx * interConst.z + dy * interConst.w;
        return new Float3(interWeight1, interWeight2, 1.0f - interWeight1 - interWeight2);
    }

    // Checks if the point is inside the triangle
    public boolean inTriangle(Float3 weights) {
        return !(weights.x < 0 || weights.y < 0 || weights.z < 0);
    }

    // Interpolates and returns the value
    private float interData(Float3 data, Float3 weights) {
        return data.x * weights.x + data.y * weights.y + data.z * weights.z;
    }
    public Vertex interVert(Float3 weights) {
        // Interpolating the world coords
        Float4 world = new Float4(
            interData(new Float3(a.world.x, b.world.x, c.world.x), weights),
            interData(new Float3(a.world.y, b.world.y, c.world.y), weights),
            interData(new Float3(a.world.z, b.world.z, c.world.z), weights),
                0.0f
        );

        // Interpolating the texture coords
        Float3 texture = new Float3(
            interData(new Float3(a.texture.x, b.texture.x, c.texture.x), weights),
            interData(new Float3(a.texture.y, b.texture.y, c.texture.y), weights),
                0.0f
        ).div(interData(new Float3(a.texture.z, b.texture.z, c.texture.z), weights));

        // Interpolating the normal
        Float3 normal = new Float3(
                interData(new Float3(a.normal.x, b.normal.x, c.normal.x), weights),
                interData(new Float3(a.normal.y, b.normal.y, c.normal.y), weights),
                interData(new Float3(a.normal.z, b.normal.z, c.normal.z), weights)
        );

        // Returning the interpolated vertex
        return new Vertex(world, texture, normal);
    }

    // Divides the triangle vertices by the world w
    public void divByW() {
        a.divByW();
        b.divByW();
        c.divByW();
    }
}
