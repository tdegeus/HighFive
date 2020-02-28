/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5EASY_HPP
#define H5EASY_HPP

#include <string>
#include <vector>

// optionally enable plug-in xtensor and load the library
#ifdef XTENSOR_VERSION_MAJOR
#ifndef H5_USE_XTENSOR
#define H5_USE_XTENSOR
#endif
#endif

#ifdef H5_USE_XTENSOR
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#endif

// optionally enable plug-in Eigen and load the library
#ifdef EIGEN_WORLD_VERSION
#ifndef H5_USE_EIGEN
#define H5_USE_EIGEN
#endif
#endif

#ifdef H5_USE_EIGEN
#include <Eigen/Eigen>
#endif

#include "H5File.hpp"

namespace H5Easy {

using HighFive::Attribute;
using HighFive::AtomicType;
using HighFive::Chunking;
using HighFive::DataSet;
using HighFive::DataSetCreateProps;
using HighFive::DataSpace;
using HighFive::Exception;
using HighFive::File;

///
/// \brief Write mode for DataSets
enum class DumpMode
{
  Create, /*!< Dump only if DataSet does not exist, otherwise throw. */
  Overwrite /*!< If DataSet already exists overwrite it if data has the same shape, otherwise throw. */
};

///
/// \brief Get the size of an existing DataSet in an open HDF5 file.
///
/// \param file Open HDF5 file, readable
/// \param path Path of the DataSet
///
/// \return Size of the DataSet
inline size_t getSize(const File& file, const std::string& path);

///
/// \brief Get the shape of an existing DataSet in an readable file.
///
/// \param file Open HDF5 file, readable
/// \param path Path of the DataSet
///
/// \return the shape of the DataSet
inline std::vector<size_t> getShape(const File& file, const std::string& path);

///
/// \brief Write an Eigen matrix to a DataSet in an open HDF5 file.
///
/// \param file Open HDF5 file, writable
/// \param path Path of the DataSet
/// \param data eigen matrix to write
/// \param mode Write mode
///
/// \return Newly created DataSet
///
#ifdef H5_USE_EIGEN
template <class T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
inline DataSet dump(File& file,
                    const std::string& path,
                    const Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols>& data,
                    DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "xt::xarray<T>" to a DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path of the DataSet
/// \param data xtensor array to write
/// \param mode Write mode
///
/// \return Newly created DataSet
///
#ifdef H5_USE_XTENSOR
template <class T>
inline DataSet dump(File& file,
                    const std::string& path,
                    const xt::xarray<T>& data,
                    DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "xt::xtensor<T,rank>" to a DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path of the DataSet
/// \param data The data to write
/// \param mode Write mode
///
/// \return Newly created DataSet
///
#ifdef H5_USE_XTENSOR
template <class T, size_t rank>
inline DataSet dump(File& file,
                    const std::string& path,
                    const xt::xtensor<T, rank>& data,
                    DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "std::vector<T>" to a DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path of the DataSet
/// \param data The data to write
/// \param mode Write mode
///
/// \return Newly created DataSet
///
template <class T>
inline DataSet dump(File& file,
                    const std::string& path,
                    const std::vector<T>& data,
                    DumpMode mode = DumpMode::Create);

///
/// \brief Write scalar/string to a DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path of the DataSet
/// \param data Data to write
/// \param mode Write mode
///
/// \return Newly created DataSet
///
template <class T>
inline DataSet dump(File& file,
                    const std::string& path,
                    const T& data,
                    DumpMode mode = DumpMode::Create);

///
/// \brief Write a scalar to a (new, extendible) DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path of the DataSet
/// \param data The data to write
/// \param idx Indices to which to write
///
/// \return Newly created DataSet
///
template <class T>
inline DataSet dump(File& file,
                    const std::string& path,
                    const T& data,
                    const std::vector<size_t>& idx);

///
/// \brief Load entry "(i,j)" from a rank-two DataSet in an open HDF5 file to a scalar.
///
/// \param file Readable opened HDF5 file
/// \param path Path of the DataSet
/// \param idx Indices to load
///
/// \return Read data
///
template <class T>
inline T load(const File& file,
              const std::string& path,
              const std::vector<size_t>& idx);

///
/// \brief Load a DataSet in an open HDF5 file to an object (templated).
///
/// \param file Readable opened HDF5 file
/// \param path Path of the DataSet
///
/// \return Read data
///
template <class T>
inline T load(const File& file, const std::string& path);

///
/// \brief Write to an Attribute of an existing DataSet in an open HDF5 file.
///
/// \param file Writeable opened HDF5 file
/// \param path Path to the DataSet
/// \param key Path of the Attribute
/// \param data Data to write
/// \param mode Write mode
///
/// \return the newly created Attribute
///
template <class T>
inline Attribute dump_attribute(File& file,
                                const std::string& path,
                                const std::string& key,
                                const T& data,
                                DumpMode mode = DumpMode::Create);

///
/// \brief Write an Eigen matrix to a DataSet in an open HDF5 file.
///
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
/// \param data eigen matrix to write
/// \param mode Write mode
///
/// \return Newly created Attribute
///
#ifdef H5_USE_EIGEN
template <class T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
inline Attribute dump_attribute(DataSet& dataset,
                                const std::string& key,
                                const Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols>& data,
                                DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "xt::xarray<T>" to a DataSet in an open HDF5 file.
///
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
/// \param data xtensor array to write
/// \param mode Write mode
///
/// \return Newly created Attribute
///
#ifdef H5_USE_XTENSOR
template <class T>
inline Attribute dump_attribute(DataSet& dataset,
                                const std::string& key,
                                const xt::xarray<T>& data,
                                DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "xt::xtensor<T,rank>" to a DataSet in an open HDF5 file.
///
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
/// \param data The data to write
/// \param mode Write mode
///
/// \return Newly created Attribute
///
#ifdef H5_USE_XTENSOR
template <class T, size_t rank>
inline Attribute dump_attribute(DataSet& dataset,
                                const std::string& key,
                                const xt::xtensor<T, rank>& data,
                                DumpMode mode = DumpMode::Create);
#endif

///
/// \brief Write "std::vector<T>" to a DataSet in an open HDF5 file.
///
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
/// \param data The data to write
/// \param mode Write mode
///
/// \return Newly created Attribute
///
template <class T>
inline Attribute dump_attribute(DataSet& dataset,
                                const std::string& key,
                                const std::vector<T>& data,
                                DumpMode mode = DumpMode::Create);

///
/// \brief Write scalar/string to an Attribute of an existing DataSet in an open HDF5 file.
///
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
/// \param data Data to write
/// \param mode Write mode
///
/// \return Newly created Attribute
///
template <class T>
inline Attribute dump_attribute(DataSet& dataset,
                                const std::string& key,
                                const T& data,
                                DumpMode mode = DumpMode::Create);

///
/// \brief Load an Attribute in an open HDF5 file to an object (templated).
///
/// \param file opened File (has to be writeable)
/// \param path Path to the DataSet
/// \param key Path of the Attribute
///
/// \return Read data
///
template <class T>
inline T load_attribute(const File& file, const std::string& path, const std::string &key);

///
/// \brief Load an Attribute in an open HDF5 file to an object (templated).
///
/// \param file opened File (has to be writeable)
/// \param dataset Opened DataSet
/// \param key Path of the Attribute
///
/// \return Read data
///
template <class T>
inline T load_attribute(const DataSet& dataset, const std::string &key);

}  // namespace H5Easy

#include "h5easy_bits/H5Easy_misc.hpp"
#include "h5easy_bits/H5Easy_scalar.hpp"
#include "h5easy_bits/H5Easy_vector.hpp"
#include "h5easy_bits/H5Easy_Eigen.hpp"
#include "h5easy_bits/H5Easy_xtensor.hpp"

#endif  // H5EASY_HPP
