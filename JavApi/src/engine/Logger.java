package engine;

import java.util.Arrays;

public class Logger {
    private static String[] logBuffer = new String[0];

    public static String[] flush() {
        String[] cpyBuff = logBuffer.clone();
        logBuffer = new String[0];
        return cpyBuff;
    }

    public static void log(String mess) {
        logBuffer = Arrays.copyOf(logBuffer, logBuffer.length + 1);
        logBuffer[logBuffer.length - 1] = mess;
    }
    public static void log(boolean val) {
        log(String.valueOf(val));
    }
    public static void log(char val) {
        log(String.valueOf(val));
    }
    public static void log(byte val) {
        log(String.valueOf(val));
    }
    public static void log(short val) {
        log(String.valueOf(val));
    }
    public static void log(int val) {
        log(String.valueOf(val));
    }
    public static void log(long val) {
        log(String.valueOf(val));
    }
    public static void log(float val) {
        log(String.valueOf(val));
    }
    public static void log(double val) {
        log(String.valueOf(val));
    }
}
