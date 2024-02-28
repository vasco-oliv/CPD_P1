#include "matrixproduct.cpp"

int main(int argc, char *argv[])
{

    char c;

    int EventSet = PAPI_NULL;
    long long values[2];
    int ret;

    // Start running
    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        std::cout << "FAIL" << endl;

    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK)
        cout << "ERROR: create eventset" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        cout << "ERROR: PAPI_L1_DCM" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        cout << "ERROR: PAPI_L2_DCM" << endl;

    // Open file
    ofstream file;
    file.open("single_mult_block_cpp.csv");

    file << "size,block_size,time,l1,l2" << endl;

    file.close();

    for (int i = 4096; i <= 10240; i += 2048)
    {
        for (int bkSize = 128; bkSize <= 512; bkSize *= 2)
        {
            for (int j = 0; j < 2; j++)
            {
                // Start counting
                ret = PAPI_start(EventSet);
                if (ret != PAPI_OK)
                    cout << "ERROR: Start PAPI" << endl;

                OnMultBlock(i, i, bkSize);

                ret = PAPI_stop(EventSet, values);

                if (ret != PAPI_OK)
                    cout << "ERROR: Stop PAPI" << endl;
                printf("L1 DCM: %lld \n", values[0]);
                printf("L2 DCM: %lld \n", values[1]);

                ret = PAPI_reset(EventSet);

                if (ret != PAPI_OK)
                    std::cout << "FAIL reset" << endl;

                fstream file2;
                file2.open("single_mult_block_cpp.csv", ios::app);

                file2 << values[0] << "," << values[1] << endl;

                file2.close();
            }
        }
    }

    // Stop running
    ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK)
        std::cout << "FAIL destroy" << endl;
}