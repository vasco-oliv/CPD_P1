import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Locale;

public class TestOnMultBlock {

    public static void onMultBlock(int m_ar, int m_br, int bkSize, FileWriter writer) throws IOException {
        long Time1, Time2;
        String st;
        int i, j, k, ii, jj, kk;

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

        for (ii = 0; ii < m_ar; ii += bkSize) {
            for (jj = 0; jj < m_br; jj += bkSize) {
                for (kk = 0; kk < m_ar; kk += bkSize) {
                    for (i = ii; i < ii + bkSize; i++) {
                        for (j = jj; j < jj + bkSize; j++) {
                            for (k = kk; k < kk + bkSize; k++) {
                                phc[i * m_ar + j] += pha[i * m_ar + k] * phb[k * m_br + j];
                            }
                        }
                    }
                }
            }
        }

        Time2 = System.currentTimeMillis();
        double t = (double) (Time2 - Time1) / 1000;
        st = String.format(Locale.US, "%s,%s,%.3f\n", m_ar, bkSize, t);
        writer.write(st);
        writer.flush();
    }

    public static void main(String[] args) {
        FileWriter writer = null;
        try {
            File file = new File("single_mult_block_java.csv");
            writer = new FileWriter(file);
            writer.write("size,block_size,time\n"); // Column headers
            for (int i = 4096; i <= 10240; i += 2048) {
                for (int bkSize = 128; bkSize <= 512; bkSize *= 2) {
                    for (int j = 0; j < 2; j++) {
                        onMultBlock(i, i, bkSize, writer);
                    }
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
