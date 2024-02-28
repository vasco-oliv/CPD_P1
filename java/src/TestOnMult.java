import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Locale;

public class TestOnMult {

    public static void onMult(int m_ar, int m_br, FileWriter writer) throws IOException {
        long Time1, Time2;
        String st;
        double temp;
        int i, j, k;

        double[] pha, phb, phc;

        pha = new double[m_ar * m_ar];
        phb = new double[m_ar * m_ar];
        phc = new double[m_ar * m_ar];

        for (i = 0; i < m_ar; i++)
            for (j = 0; j < m_ar; j++)
                pha[i * m_ar + j] = 1.0;

        for (i = 0; i < m_br; i++)
            for (j = 0; j < m_br; j++)
                phb[i * m_br + j] = i + 1;

        Time1 = System.currentTimeMillis();

        for (i = 0; i < m_ar; i++) {
            for (j = 0; j < m_br; j++) {
                temp = 0;
                for (k = 0; k < m_ar; k++) {
                    temp += pha[i * m_ar + k] * phb[k * m_br + j];
                }
                phc[i * m_ar + j] = temp;
            }
        }

        Time2 = System.currentTimeMillis();
        double t = (double) (Time2 - Time1) / 1000;
        st = String.format(Locale.US, "%s,%.3f\n", m_ar, t);
        writer.write(st);
        writer.flush();
    }

    public static void main(String[] args) {
        FileWriter writer = null;
        try {
            File file = new File("single_mult_java.csv");
            writer = new FileWriter(file);
            writer.write("size,time\n"); // Column headers
            for (int i = 600; i <= 3000; i += 400) {
                for (int j = 0; j < 2; j++) {
                    onMult(i, i, writer);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (writer != null) {
                try {
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}