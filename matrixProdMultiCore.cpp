#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>
#include <omp.h>
#include <fstream>
#include <chrono>

using namespace std;

#define SYSTEMTIME clock_t

void OnMultLine(int m_ar, int m_br)
{
    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (i = 0; i < m_ar; i++)
        for (j = 0; j < m_ar; j++)
        {
            pha[i * m_ar + j] = (double)1.0;
            phc[i * m_ar + j] = (double)0.0;
        }

    for (i = 0; i < m_br; i++)
        for (j = 0; j < m_br; j++)
            phb[i * m_br + j] = (double)(i + 1);

    auto Time1 = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for private(i, j, k, temp)
    for (i = 0; i < m_ar; i++)
    {
        for (j = 0; j < m_br; j++)
        {   temp = 0;
            #pragma omp parallel
            for (k = 0; k < m_ar; k++)
            {
                temp = pha[i * m_ar + j] * phb[j * m_br + k];
                phc[i * m_ar + k] += temp;
            }
        }
    }

    auto Time2 = std::chrono::high_resolution_clock::now();
    double t = std::chrono::duration_cast<std::chrono::milliseconds>(Time2 - Time1).count() / 1000.0;
    sprintf(st, "Time: %3.3f seconds\n", t);
    cout << st;

    fstream file;
    file.open("single_mult_line_multi_core_1.csv", ios::app);

    file << m_ar << "," << t << ",";

    file.close(); 

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < min(10, m_br); j++)
            cout << phc[j] << " ";
    }
    cout << endl;

    free(pha);
    free(phb);
    free(phc);
}

void handle_error(int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

void init_papi()
{
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT && retval < 0)
    {
        printf("PAPI library version mismatch!\n");
        exit(1);
    }
    if (retval < 0)
        handle_error(retval);

    std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
              << " MINOR: " << PAPI_VERSION_MINOR(retval)
              << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}

// int main(int argc, char *argv[])
// {

//     char c;
//     int lin, col, blockSize;
//     int op;

//     int EventSet = PAPI_NULL;
//     long long values[2];
//     int ret;

//     ret = PAPI_library_init(PAPI_VER_CURRENT);
//     if (ret != PAPI_VER_CURRENT)
//         std::cout << "FAIL" << endl;

//     ret = PAPI_create_eventset(&EventSet);
//     if (ret != PAPI_OK)
//         cout << "ERROR: create eventset" << endl;

//     ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
//     if (ret != PAPI_OK)
//         cout << "ERROR: PAPI_L1_DCM" << endl;

//     ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
//     if (ret != PAPI_OK)
//         cout << "ERROR: PAPI_L2_DCM" << endl;

//     op = 1;

//     printf("Dimensions: lins=cols ? ");
//     cin >> lin;
//     col = lin;

//     // Start counting
//     ret = PAPI_start(EventSet);
//     if (ret != PAPI_OK)
//         cout << "ERROR: Start PAPI" << endl;

//     OnMultLine(lin, col);

//     ret = PAPI_stop(EventSet, values);
//     if (ret != PAPI_OK)
//         cout << "ERROR: Stop PAPI" << endl;
//     //printf("L1 DCM: %lld \n", values[0]);
//     //printf("L2 DCM: %lld \n", values[1]);

//     ret = PAPI_reset(EventSet);
//     if (ret != PAPI_OK)
//         std::cout << "FAIL reset" << endl;

//     ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
//     if (ret != PAPI_OK)
//         std::cout << "FAIL remove event" << endl;

//     ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
//     if (ret != PAPI_OK)
//         std::cout << "FAIL remove event" << endl;

//     ret = PAPI_destroy_eventset(&EventSet);
//     if (ret != PAPI_OK)
//         std::cout << "FAIL destroy" << endl;
// }


int main (int argc, char *argv[])
{

	char c;
	
	int EventSet = PAPI_NULL;
  	long long values[2];
  	int ret;
	
    // Start running
	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAIL" << endl;

	ret = PAPI_create_eventset(&EventSet);
		if (ret != PAPI_OK) cout << "ERROR: create eventset" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;

    ret = PAPI_add_event(EventSet,PAPI_DP_OPS);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_DP_OPS" << endl;

    ret = PAPI_add_event(EventSet,PAPI_L2_DCA );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_DP_OPS" << endl;



    

    

	// Open file
	ofstream file;
	file.open("single_mult_line_multi_core_1.csv");

	file << "size,time,l1,l2,flops" << endl;

	file.close();

    for (int i = 4096; i <= 10240; i += 2048) {
        for (int j = 0; j < 2; j++) {

            // Start counting
            ret = PAPI_start(EventSet);
            if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

            OnMultLine(i, i);

            ret = PAPI_stop(EventSet, values);
            
            if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
            // printf("L1 DCM: %lld \n",values[0]);
            // printf("L2 DCM: %lld \n",values[1]);
            // printf("FLOPS: %lld \n",values[2]);
            // printf("cache 2 acceses: %lld \n",values[3]);


            ret = PAPI_reset( EventSet );
            
            if ( ret != PAPI_OK )
                std::cout << "FAIL reset" << endl;
			
			fstream file2;
			file2.open("single_mult_line_multi_core.csv", ios::app);

			file2 << values[0] << "," << values[1] << ',' << values[2] << endl;

			file2.close();
        }
    }

    // Stop running
    ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 

    ret = PAPI_remove_event( EventSet, PAPI_DP_OPS );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 



    ret = PAPI_remove_event( EventSet, PAPI_L2_DCA );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl; 


    	ret = PAPI_destroy_eventset( &EventSet );
	if ( ret != PAPI_OK )
		std::cout << "FAIL destroy" << endl;
        

         


}