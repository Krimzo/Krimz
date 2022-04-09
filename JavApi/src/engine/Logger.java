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
}
