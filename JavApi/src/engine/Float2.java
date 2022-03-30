package engine;

public class Float2 {
    public float x;
    public float y;

    public Float2(float a) {
        x = a;
        y = a;
    }
    public Float2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }

    // Length
    public float len() {
        return (float)Math.sqrt(x * x + y * y);
    }

    // Normalization
    public Float2 norm() {
        return div(len());
    }

    // Addition
    public Float2 add(Float2 v) {
        return new Float2(x + v.x, y + v.y);
    }

    // Subtraction
    public Float2 sub(Float2 v) {
        return new Float2(x - v.x, y - v.y);
    }

    // Multiplication
    public Float2 mul(float a) {
        return new Float2(x * a, y * a);
    }
    public Float2 mul(Float2 v) {
        return new Float2(x * v.x, y * v.y);
    }

    // Division
    public Float2 div(float a) {
        return mul(1.0f / a);
    }
    public Float2 div(Float2 v) {
        return new Float2(x / v.x, y / v.y);
    }

    // Negation
    public Float2 negate() {
        return mul(-1.0f);
    }

    // Dot product
    public float dot(Float2 a) {
        return x * a.x + y * a.y;
    }

    // Angle between vectors
    public float angle(Float2 a) {
        return (float)Math.toDegrees(Math.acos(this.norm().dot(a.norm())));
    }

    // Returns a rotated vector around the given point
    public Float2 rotate(float angle) {
        final float sinA = (float)Math.sin(Math.toRadians(angle));
        final float cosA = (float)Math.cos(Math.toRadians(angle));
        return new Float2(cosA * x - sinA * y, sinA * x + cosA * y);
    }
}
