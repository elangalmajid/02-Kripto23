#include <iostream>
#include <fstream>
#include <math.h>

typedef int Matrix[100][100];

void inputMatrix(Matrix m, int p, int l)
{
    for (int i = 0; i < p; i++)
    {
        std::cout << "Baris ke - " << i + 1 << "\n";
        for (int j = 0; j < l; j++)
        {
            std::cout << "\tKolom ke - " << j + 1 << ":";
            std::cin >> m[i][j];
        }
        std::cout << "\n";
    }
}

void printMatrix(Matrix m, int p, int l)
{
    for (int i = 0; i < p; i++)
    {
        std::cout << "[ ";
        for (int j = 0; j < l; j++)
        {
            std::cout << m[i][j] << " ";
        }
        std::cout << "]\n";
    }
}

void printArray(int arr[], int p)
{
    for (int i = 0; i < p; i++)
    {
        std::cout << "[" << arr[i] << "]"
                  << "\n";
    }
}

void mulMat(Matrix a, Matrix b, Matrix c, int pa, int la, int pb)
{
    for (int i = 0; i < pa; i++)
    {
        for (int j = 0; j < la; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < pb; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void mulMatArr(Matrix a, int b[], int c[], int pa, int la)
{
    if (pa == 2)
    {
        for (int i = 0; i < pa; i++)
        {
            c[i] = 0;
            for (int k = 0; k < pa; k++)
            {
                c[i] += a[k][i] * b[k];
            }
        }
    }
    else
    {
        for (int i = 0; i < pa; i++)
        {
            c[i] = 0;
            for (int k = 0; k < pa; k++)
            {
                c[i] += a[i][k] * b[k];
            }
        }
    }
}

std::string encrypt(std::string ptext, Matrix key, int p, int l)
{
    int b[100];
    int c[100];
    int stop;
    std::string enc = "";
    int si = 0;
    if (ptext.length() % p == 0)
        stop = ptext.length() / p;
    else
        stop = ptext.length() / p + 1;

    for (int i = 0; i < stop; i++)
    {
        int j;
        for (int j = 0; j < l; j++)
        {
            if (ptext[si] == '\0')
            {
                b[j] = 0;
            }

            else
            {
                b[j] = (int)(ptext[si] - 'A');
            }

            si++;
        }

        mulMatArr(key, b, c, p, l);

        for (int j = 0; j < p; j++)
        {

            enc = enc + (char)((c[j] % 26) + 'A');
        }
    }

    return enc;
}
void getCofactor(Matrix A, Matrix temp, int p, int q,
                 int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those
            //  element which are not in given row and
            //  column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
int determinant(Matrix A, int n)
{
    int D = 0; // Initialize result

    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];

    Matrix temp; // To store cofactors

    int sign = 1; // To store sign multiplier

    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return D;
}

// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(Matrix A, Matrix adj, int N)
{
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of A[][]
    int sign = 1;
    Matrix temp;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign) * (determinant(temp, N - 1));
        }
    }
}

void MatrixOperation(Matrix m, int a, int p, int l, char op)
{
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < l; j++)
        {
            switch (op)
            {
            case '+':
                m[i][j] = m[i][j] + a;
                break;
            case '*':
                m[i][j] = m[i][j] * a;
                break;
            case '%':
                if (m[i][j] < 0)
                {
                    m[i][j] = (m[i][j] % a) + 26;
                }

                else
                {
                    m[i][j] = m[i][j] % a;
                }
                break;

            default:
                break;
            }
        }
    }
}

std::string decrypt(std::string ctext, Matrix key, int p, int l)
{
    std::string dec = "";
    int d = determinant(key, p) % 26;
    int i = 1;
    int b[100];
    int c[100];
    int si = 0;
    Matrix m;

    adjoint(key, m, p);
    while ((d * i) % 26 != 1)
    {
        i++;
    }
    MatrixOperation(m, i, p, l, '*');
    MatrixOperation(m, 26, p, l, '%');

    int stop;
    if (ctext.length() % p == 0)
        stop = ctext.length() / p;
    else
        stop = ctext.length() / p + 1;
    for (int i = 0; i < stop; i++)
    {
        for (int j = 0; j < l; j++)
        {
            b[j] = (int)(ctext[si] - 'A');
            si++;
        }
        mulMatArr(m, b, c, p, l);

        for (int j = 0; j < p; j++)
        {
            dec = dec + (char)((c[j] % 26) + 'A');
        }
    }

    return dec;
}

int main()
{
    std::string ptext;
    int p, l;

    std::cout << "=== PLAIN TEXT ===\n";
    std::cout << "Plain text : ";
    std::cin >> ptext;

    std::cout << "=== KEY === \n";
    std::cout << "Panjang Matriks : ";
    std::cin >> p;
    std::cout << "Lebar Matriks : ";
    std::cin >> l;
    Matrix key;

    std::cout << "=== INPUT MATRIX ===\n";
    inputMatrix(key, p, l);

    if (p != l || determinant(key, p) == 0)
    {
        std::cout << "Matriks : \n";
        printMatrix(key, p, l);
        std::cout << "Tidak dapat digunakan sebagai kunci karena : ";

        if (p != l)
        {
            std::cout << "bukan matriks persegi ";
        }
        if (determinant(key, p) == 0)
        {
            std::cout << "tidak memiliki invers ";
        }
        std::cout << "\n";
    }

    else
    {

        std::string enc = encrypt(ptext, key, p, l);
        std::string dec = decrypt(enc, key, p, l);

        std::ofstream MyFile1("encrypted.txt");
        std::ofstream MyFile2("decrypted.txt");

        // Write to the file
        int i = 0;
        while (i < ptext.length())
        {
            MyFile1 << enc[i];
            MyFile2 << dec[i];
            i++;
        }

        // Close the file
        MyFile1.close();
        MyFile2.close();
    }
}