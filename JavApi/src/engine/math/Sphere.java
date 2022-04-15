package engine.math;

public class Sphere {
    public Float3 center;
    public float radius;

    public Sphere(Float3 center, float radius) {
        this.center = center;
        this.radius = radius;
    }
    public Sphere(Sphere s) {
        center = new Float3(s.center);
        radius = s.radius;
    }
}
