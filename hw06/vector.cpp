#include "vector.h"

#include <iterator>
#include <numeric>
#include <cmath>


namespace linalg {
auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

Vector::Vector(std::size_t n) : data_(n) {}

Vector::Vector(std::size_t n, float val) : data_(n, val) {}

Vector::Vector(std::initializer_list<float> list) : data_(list) {}

Vector& Vector::operator=(float val)
{
    std::fill(data_.begin(), data_.end(), val);
    return *this;
}

void Vector::assign(float val)
{
    std::fill(data_.begin(), data_.end(), val);
}

void Vector::assign(Vector v)
{
    data_ = std::move(v.data_);
}

std::size_t Vector::size() const
{
    return data_.size();
}

Vector::iterator Vector::begin()
{
    return data_.begin();
}

Vector::iterator Vector::end()
{
    return data_.end(); 
}

Vector::const_iterator Vector::begin() const
{
    return data_.cbegin();
}

Vector::const_iterator Vector::end() const
{
    return data_.cend();
}

Vector::const_iterator Vector::cbegin() const
{
    return data_.cbegin();
}

Vector::const_iterator Vector::cend() const
{
    return data_.cend();
}

float& Vector::operator[](int idx)
{
    if (idx >= 0)
    {
        return data_[idx];
    } else 
    {
        return data_[data_.size() + idx];
    }
}

const float& Vector::operator[](int idx) const
{
    if (idx >= 0)
    {
        return data_[idx];
    } else 
    {
        return data_[data_.size() + idx];
    }
}

float& Vector::coeff(int idx)
{
    if (static_cast<std::size_t>(idx) >= data_.size() || idx < 0)
    {
        throw std::out_of_range{"idx out of range"};
    }

    return data_[idx];
}

const float& Vector::coeff(int idx) const
{
    if (static_cast<std::size_t>(idx) >= data_.size() || idx < 0)
    {
        throw std::out_of_range{"idx out of range"};
    }

    return data_[idx];
}

Vector& Vector::operator+=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [&](auto element) {
            return element + val;
        }
    );
    return *this;
}

Vector& Vector::operator-=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [&](auto element) {
            return element - val;
        }
    );
    return *this;
}

Vector& Vector::operator*=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [&](auto element) {
            return element * val;
        }
    );
    return *this;
}

Vector& Vector::operator/=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [&](auto element) {
            return element / val;
        }
    );
    return *this;
}

Vector& Vector::operator+=(const Vector& y)
{
    if (y.size() != this->size())
    {
        throw std::invalid_argument{"mismatching input vector size."};
    }
    std::transform(data_.begin(), data_.end(), y.begin(), data_.begin(),
        [](auto a, auto b) {
            return a + b;
        }
    );
    return *this;
}

Vector& Vector::operator-=(const Vector& y)
{
    if (y.size() != this->size())
    {
        throw std::invalid_argument{"mismatching input vector size."};
    }
    std::transform(data_.begin(), data_.end(), y.begin(), data_.begin(),
        [](auto a, auto b) {
            return a - b;
        }
    );
    return *this;
}

float min(const Vector& x)
{
    if (x.size() == 0)
    {
        throw std::invalid_argument{"vector has size 0. Cannot calculate minimum."};
    }
    return *std::min_element(x.begin(), x.end());
}

float max(const Vector& x)
{
    if (x.size() == 0)
    {
        throw std::invalid_argument{"vector has size 0. Cannot calculate maximum."};
    }
    return *std::max_element(x.begin(), x.end());
}

std::size_t argmin(const Vector& x)
{
    if (x.size() == 0)
    {
        throw std::invalid_argument{"vector has size 0. Cannot calculate argmin."};
    }
    return std::distance(x.begin(), std::min_element(x.begin(), x.end()));
}

std::size_t argmax(const Vector& x)
{
    if (x.size() == 0)
    {
        throw std::invalid_argument{"vector has size 0. Cannot calculate argmax."};
    }
    return std::distance(x.begin(), std::max_element(x.begin(), x.end()));
}

std::size_t non_zeros(const Vector& x)
{
    std::size_t count{0};
    std::for_each(x.begin(), x.end(), 
        [&count](auto element) {
            count += (element != 0);
        }
    );
    return count;
}

float sum(const Vector& x)
{
    return std::accumulate(x.begin(), x.end(), 0.0f);
}

float prod(const Vector& x)
{
    return std::accumulate(x.begin(), x.end(), 1.0f, 
        [](auto a, auto b) {
            return a * b;
        }
    );
}

float dot(const Vector& x, const Vector& y)
{
    return std::inner_product(x.begin(), x.end(), y.begin(), 0.0f);
}

float norm(const Vector& x)
{
    float norm{0.0f};
    std::for_each(x.begin(), x.end(), 
        [&norm](auto element) {
            norm += element*element;
        }
    );
    return norm;
}

void normalize(Vector& x)
{
    x /= static_cast<float>(sqrt((norm(x))));
}

Vector normalized(const Vector& x)
{
    Vector result(x.size());
    result.assign(x);
    normalize(result);

    return result;
}

Vector floor(const Vector& x)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [](auto element) {
            return std::floor(element);
        }
    );
    return result;
}

Vector ceil(const Vector& x)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [](auto element) {
            return std::ceil(element);
        }
    );
    return result;
}

Vector operator+(const Vector& x)
{
    Vector result(x.size());
    result.assign(x);
    return result;
}

Vector operator-(const Vector& x)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [](auto element) {
            return -element;
        }
    );
    return result;
}

Vector operator+(const Vector& x, const Vector& y)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), y.begin(), result.begin(), 
        [](auto a, auto b) {
            return a + b;
        }
    );
    return result;
}

Vector operator-(const Vector& x, const Vector& y)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), y.begin(), result.begin(), 
        [](auto a, auto b) {
            return a - b;
        }
    );
    return result;
}

Vector operator+(const Vector& x, float val)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [&val](auto element) {
            return element + val;
        }
    );
    return result;
}

Vector operator-(const Vector& x, float val)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [&val](auto element) {
            return element - val;
        }
    );
    return result;
}

Vector operator*(const Vector& x, float val)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [&val](auto element) {
            return element * val;
        }
    );
    return result;
}

Vector operator/(const Vector& x, float val)
{
    Vector result(x.size());
    std::transform(x.begin(), x.end(), result.begin(), 
        [&val](auto element) {
            return element / val;
        }
    );
    return result;
}

Vector operator+(float val, const Vector& x)
{
    return operator+(x, val);
}

Vector operator-(float val, const Vector& x)
{
    return operator-(x, val);
}

Vector operator*(float val, const Vector& x)
{
    return operator*(x, val);
}



}
