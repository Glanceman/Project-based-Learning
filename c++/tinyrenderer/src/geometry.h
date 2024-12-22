#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <optional>
#include <ostream>
#include <expected>
#include <vector>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, int n>
struct Vec
{
    T data[n] = {0};

    T &operator[](const int i)
    {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T norm2() const { return *this * *this; }
    T norm() const { return std::sqrt(norm2()); }
};

template <typename T, int n>
Vec<T, n> operator+(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] += rhs[i]);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator-(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] -= rhs[i]);
    return ret;
}

template <typename T, int n>
T operator*(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    T ret = 0;
    for (int i = n; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator*(const T &rhs, const Vec<T, n> &lhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator*(const Vec<T, n> &lhs, const T &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator/(const Vec<T, n> &lhs, const T &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] /= rhs);
    return ret;
}

template <typename T, int n1, int n2>
Vec<T, n1> embed(const Vec<T, n2> &v, T fill = 1)
{
    Vec<T, n1> ret;
    for (int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
    return ret;
}

template <typename T, int n1, int n2>
Vec<T, n1> proj(const Vec<T, n2> &v)
{
    Vec<T, n1> ret;
    for (int i = n1; i--; ret[i] = v[i]);
    return ret;
}

template <typename T, int n>
std::ostream &operator<<(std::ostream &out, const Vec<T, n> &v)
{
    for (int i = 0; i < n; i++) out << v[i] << " ";
    return out;
}

template <typename T, int n>
bool operator==(const Vec<T, n> &rhs, const Vec<T, n> &lhs)
{
    for (int i = 0; i<n; i++){
        if(rhs[i]!=lhs[i]) return false;
    }
    return true;
}


template <typename T>
struct Vec<T, 2>
{
    union
    {
        struct
        {
            T u, v;
        };
        struct
        {
            T x, y;
        };
    };

    constexpr Vec() noexcept :
        u(0), v(0) {}

    constexpr Vec(T _u, T _v) noexcept :
        u(_u), v(_v) {}

    T &operator[](const int i)
    {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }

    inline T         norm2() const { return *this * *this; }
    inline T         norm() const { return std::sqrt(norm2()); }
    inline Vec<T, 2> normalized() { return (*this) / norm(); }
    inline T         dot(const Vec<T, 2> &vec) const { return *this * vec; }
    inline Vec<T, 2> dot(T scalar) const { return *this * scalar; }
    inline T         cross(const Vec<T, 2> &vec) const { return ((*this).u * vec.v - (*this).v * vec.u); }
};

template <typename T>
struct Vec<T, 3>
{
    T x = 0, y = 0, z = 0;

    constexpr Vec() noexcept = default;

    constexpr Vec(T _x, T _y, T _z) noexcept :
        x(_x), y(_y), z(_z) {}

    T &operator[](const int i)
    {
        assert(i >= 0 && i < 3);
        return i ? (1 == i ? y : z) : x;
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < 3);
        return i ? (1 == i ? y : z) : x;
    }

    inline T         norm2() const { return *this * *this; }
    inline T         norm() const { return std::sqrt(norm2()); }
    inline Vec<T, 3> normalized() { return (*this) / norm(); }
    inline T         dot(const Vec<T, 3> &vec) const { return *this * vec; }
    inline Vec<T, 3> cross(const Vec<T, 3> &v) const
    {
        return Vec<T, 3>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

// Type definitions with default type double
using Vec2  = Vec<double, 2>;
using Vec2f = Vec<float, 2>;
using Vec2i = Vec<int, 2>;
using Vec3  = Vec<double, 3>;
using Vec3f = Vec<float, 3>;
using Vec3i = Vec<int, 3>;
using Vec4  = Vec<double, 4>;
using Vec4f = Vec<float, 4>;
using Vec4i = Vec<int, 4>;

template <typename T, int rows, int cols>
struct Mat
{
    std::array<Vec<T, cols>, rows> data{{}};

    inline size_t get_rows() const { return rows; }
    inline size_t get_cols() const { return cols; }

    Vec<T, cols> &operator[](const int idx)
    {
        assert(idx >= 0 && idx < data.size());
        return data[idx];
    }
    const Vec<T, cols> &operator[](const int idx) const
    {
        assert(idx >= 0 && idx < data.size());
        return data[idx];
    }

    /// @brief get the col values
    Vec<T, cols> col(const int idx) const
    {
        assert(idx >= 0 && idx < cols);
        Vec<T, cols> ret;
        for (int row = rows; row > 0; --row) ret[row] = data[row][idx];
        return ret;
    }

    void set_col(const int idx, const Vec<T, cols> &v)
    {
        assert(idx >= 0 && idx < cols);
        for (int row = rows; row > 0; row--) data[row][idx] = v[row];
    }

    static Mat<T,rows,cols> identity() {
        Mat<T,rows,cols> ret;
        for (int i=rows; i--; )
            for (int j=cols;j>0; j--){
                if (i==j){
                    ret[i][j]=1;
                }
            }   
        return ret;
    }

    template<int row_size,int col_size>
    std::optional<Mat<T, row_size, col_size>>sub_mat(int r_index,int c_index) const {
        // checking
        if( r_index+row_size > rows || c_index+col_size > cols) return std::nullopt;
        Mat<T, row_size, col_size> mat;
        for(int r=r_index; r<(r_index+row_size); r++){
            for(int c=c_index; c<(c_index+col_size); c++){
                mat[r-r_index][c-c_index]=data[r][c];
            }
        }
        return mat;
    }
    
    double det(){
        if(rows!=cols) return -1;
        double value =0;
        value = det_recur(*this);
        return value;
    }

    Mat<T, rows, cols> tranpose(){
        Mat<T, cols, rows> res;
        for(size_t r=0; r< res.get_rows(); r++){
            for(size_t c=0; c<res.get_cols(); c++){
                res[r][c]=(*this)[c][r];
            }
        }
        return res;
    }
    
private:
    template<typename t, int nr, int nc>
    double det_recur(const Mat<t, nr, nc>& mat) {
        char sign_bit = 0;
        if (nr == 1 && nc == 1) return mat[0][0];

        double det = 0;
        for (int col = 0; col < nc; col++) {
            // Define dimensions for the submatrix
            // Manually create the submatrix
            Mat<t, nr-1, nc-1> sub_mat;
            for (int r = 1; r < nr; r++) {
                int sub_r_idx = r - 1;
                for (int c = 0; c < nc; c++) {
                    if (c == col) continue;
                    int sub_c_idx = (c < col) ? c : c - 1;
                    sub_mat[sub_r_idx][sub_c_idx] = mat[r][c];
                }
            }

            // Determine the sign for this term
            double sign = (sign_bit % 2 == 0) ? 1 : -1;

            // Recursively calculate the determinant of the submatrix
            double sub_det = det_recur(sub_mat);

            // Accumulate the determinant
            det += sign * mat[0][col] * sub_det;

            // Toggle the sign_bit for the next iteration
            sign_bit ^= 1;
        }
        return det;
    }

    template<typename t, int nr=1, int nc=1>
    double det_recur(const Mat<t, 1, 1>& mat) {
        return mat[0][0];
    }

};


template<typename T, int rows,int cols>
Mat<T,rows,cols> operator*(const Mat<T,rows,cols>& lhs, const double& val) {
    Mat<T,rows,cols> result;
    for (int i=rows; i--; result[i] = lhs[i]*val);
    return result;
}

template<typename T, int rows,int cols> 
Vec<T, rows> operator*(const Mat<T,rows,cols>& lhs, const Vec<T,cols>& rhs) {
    Vec<T,rows> ret;
    for (int i=rows;i>0; i--){ 
        ret[i] = lhs[i]*rhs;
    }
    return ret;
}

template<typename T, int rows_1,int cols_1, int rows_2, int cols_2> 
Mat<T, rows_1, cols_2> operator*(const Mat<T,rows_1,cols_1>& lhs, const Mat<T,rows_2,cols_2>& rhs) {
    Mat<T, rows_1, cols_2> ret;
    for (int i=rows_1;i>0; i--){
        for(int j=cols_2; j>0;j--){
            ret[i] = lhs[i]*rhs.col(j);
        } 
    }
    return ret;
}


template<typename T, int rows,int cols>
Mat<T,rows,cols> operator/(const Mat<T,rows,cols>& lhs, const double& val) {
    Mat<T,rows,cols> result;
    for (int i=rows; i--; result[i] = lhs[i]/val);
    return result;
}


template<typename T, int rows_1,int cols_1, int rows_2, int cols_2> 
Mat<T, rows_1, cols_2> operator+(const Mat<T,rows_1,cols_1>& lhs, const Mat<T,rows_2,cols_2>& rhs) {
    Mat<T, rows_1, cols_2> ret;
    for (int i=rows_1;i>0; i--){
        ret[i] = lhs[i] + rhs[i];

    }
    return ret;
}

template<typename T, int rows_1,int cols_1, int rows_2, int cols_2> 
Mat<T, rows_1, cols_2> operator-(const Mat<T,rows_1,cols_1>& lhs, const Mat<T,rows_2,cols_2>& rhs) {
    Mat<T, rows_1, cols_2> ret;
    for (int i=rows_1;i>0; i--){
        ret[i] = lhs[i] + rhs[i];

    }
    return ret;
}

template<typename T, int rows,int cols>
 std::ostream& operator<<(std::ostream& out, const Mat<T,rows,cols>& m) {
    for (int i=0; i<rows; i++) out << m[i] << std::endl;
    return out;
}

template<typename T, int rows_1,int cols_1, int rows_2, int cols_2> 
bool operator==(const Mat<T,rows_1,cols_1>& lhs, const Mat<T,rows_2,cols_2>& rhs) {
    bool res = true;
    if(rows_1!=rows_2 || cols_1!=cols_2){ return res=false;}
    for(size_t r=0; r<lhs.get_rows(); r++){
        for(size_t c=0; c<lhs.get_cols();c++){
            if(lhs[r][c]!=rhs[r][c]){
                res = false;
                break;
            }
        }
    }
    return res;
}





#endif //__GEOMETRY_H__