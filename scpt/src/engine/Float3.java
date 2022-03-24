package engine;

public class Float3 {
    public float x;
    public float y;
    public float z;

    @Override
    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }

    public Float3(float a) {
        x = a;
        y = a;
        z = a;
    }
    public Float3(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public Float3(Float2 v, float z) {
        x = v.x;
        y = v.y;
        this.z = z;
    }
    public Float3(float x, Float2 v) {
        this.x = x;
        y = v.x;
        z = v.y;
    }

    // xy getter
    public Float2 xy() {
        return new Float2(x, y);
    }

    // Length
    public float len() {
        return (float)Math.sqrt(x * x + y * y + z * z);
    }

    // Normalization
    public Float3 norm() {
        return div(len());
    }

    // Addition
    public Float3 add(Float3 v) {
        return new Float3(x + v.x, y + v.y, z + v.z);
    }

    // Subtraction
    public Float3 sub(Float3 v) {
        return new Float3(x - v.x, y - v.y, z - v.z);
    }

    // Multiplication
    public Float3 mul(float a) {
        return new Float3(x * a, y * a, z * a);
    }
    public Float3 mul(Float3 v) {
        return new Float3(x * v.x, y * v.y, z * v.z);
    }

    // Division
    public Float3 div(float a) {
        return mul(1 / a);
    }
    public Float3 div(Float3 v) {
        return new Float3(x / v.x, y / v.y, z / v.z);
    }

    // Negation
    public Float3 negate() {
        return mul(-1);
    }

    // Dot product
    public float dot(Float3 a) {
        return x * a.x + y * a.y + z * a.z;
    }

    // Cross product
    public Float3 cross(Float3 a) {
        return new Float3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
    }

    // Angle between vectors
    public float angle(Float3 a) {
        return (float)Math.toDegrees(Math.acos(this.norm().dot(a.norm())));
    }

    // Returns a rotated vector around the given axis
    private static final float halfAngleRad = (float)(Math.PI / 360.0f);
    public Float3 rotate(float angle, Float3 axis) {
        // Calculating trig values
        final float angleSin = (float)Math.sin(angle * halfAngleRad);
        final float angleCos = (float)Math.cos(angle * halfAngleRad);

        // Calculating quaternion constants
        final float qx = axis.x * angleSin;
        final float qy = axis.y * angleSin;
        final float qz = axis.z * angleSin;
        final float x2 = qx * qx;
        final float y2 = qy * qy;
        final float z2 = qz * qz;
        final float w2 = angleCos * angleCos;
        final float xy = qx * qy;
        final float xz = qx * qz;
        final float yz = qy * qz;
        final float xw = qx * angleCos;
        final float yw = qy * angleCos;
        final float zw = qz * angleCos;

        // Calculating the rotated vector
        Float3 temp = new Float3(0);
        temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z;
        temp.y = (xy + zw + zw + xy) * x + ( y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z;
        temp.z = (xz - yw + xz - yw) * x + ( yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z;
        return temp;
    }
}
