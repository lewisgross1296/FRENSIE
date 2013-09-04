//---------------------------------------------------------------------------//
//!
//! \file HDF5FileHandler_def.hpp
//! \author Alex Robinson
//! \brief HDF5 file handler template member function definitions
//!
//---------------------------------------------------------------------------//

#ifndef HDF5_FILE_HANDLER_DEF_HPP
#define HDF5_FILE_HANDLER_DEF_HPP

//Std Lib Includes
#include <string>

// Trilinos Includes
#include <Teuchos_Array.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayView.hpp>
#include <Teuchos_TwoDArray.hpp>

// HDF5 includes
#include <H5Cpp.h>

// FACEMC includes
#include "HDF5TypeTraits.hpp"
#include "HDF5ArrayPolicy.hpp"
#include "ExceptionCatchMacros.hpp"
#include "FACEMC_Assertion.hpp"
#include "ContractException.hpp"

namespace FACEMC{

// Write data in array to HDF5 file dataset
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in] data The data array to write to the HDF5 file dataset.
 * \param[in] location_in_file The location in the HDF5 file where the data will
 * \pre A valid location string, which is any string that does not start with
 * a "/", must be given to this function.
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::writeArrayToDataSet( const Array<T> &data,
					   const std::string &location_in_file
					   )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( location_in_file.compare( 0, 1, "/" ) == 0 ); 
  
  // Create any parent groups that do not exist yet in the location path
  createParentGroups( location_in_file );
  
  // HDF5 exceptions can be thrown when creating a dataset or writing to a 
  // dataset
  try
  {
    hsize_t dim = HDF5ArrayPolicy<T,Array>::size(data);
    H5::DataSpace space( 1, &dim );
    H5::DataSet dataset(d_hdf5_file->createDataSet( 
						 location_in_file,
						 HDF5TypeTraits<T>::dataType(),
						 space ) );
    dataset.write( HDF5ArrayPolicy<T,Array>::getRawPtr(data), 
		   HDF5TypeTraits<T>::dataType() );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Read in HDF5 file dataset and save the data to an array
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in,out] data The data array that will be used to store the HDF5 file 
 * dataset.
 * \param[in] location_in_file The location in the HDF5 file where the data will
 * be read from.
 * \pre A valid location string, which is any string that does not start with
 * a "/", must be given to this function.
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::readArrayFromDataSet( Array<T> &data,
					    const std::string &location_in_file
					    )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( location_in_file.compare( 0, 1, "/" ) == 0 ); 
  
  // HDF5 exceptions can be thrown when opening and reading from datasets
  try
  {
    H5::DataSet dataset(d_hdf5_file->openDataSet( location_in_file ) );
    
    // Get the dataspace of the dataset
    H5::DataSpace dataspace = dataset.getSpace();
    
    // Get the number of dimensions in the dataspace
    int rank = dataspace.getSimpleExtentNdims();
    
    // Get the dimension size of each dimension in the dataspace
    hsize_t dims[rank];
    int ndims = dataspace.getSimpleExtentDims( dims, NULL );
    
    // Resize the output array to the size of the dataspace
    typename Array<T>::size_type size = dims[0];
    for( unsigned int i = 1; i < rank; ++i )
      size *= dims[i];
    
    HDF5ArrayPolicy<T,Array>::resize( data, size );
    
    // Read the data in the dataset and save it to the output array
    dataset.read( HDF5ArrayPolicy<T,Array>::getRawPtr(data),
		  HDF5TypeTraits<T>::dataType() );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Write attribute to HDF5 file dataset
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in] data The data array to write to the HDF5 file dataset attribute.
 * \param[in] location_in_file The location in the HDF5 file where the dataset
 * attribute will written.
 * \param[in] attribute_name The name of the dataset attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::writeArrayToDataSetAttribute( const Array<T> &data,
						    const std::string 
						      &dataset_location,
						    const std::string
						      &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (dataset_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
    
  // HDF5 exceptions can be thrown when opening a group, creating an attribute,
  // or writing an attribute to a group
  try
  {
    hsize_t dim = HDF5ArrayPolicy<T,Array>::size(data);
    H5::DataSpace space( 1, &dim );
    H5::DataSet dataset(d_hdf5_file->openDataSet( dataset_location ) ); 
    H5::Attribute attribute(dataset.createAttribute( 
					  attribute_name, 
					  HDF5TypeTraits<T>::dataType(),
					  space ) );
    
    attribute.write( HDF5TypeTraits<T>::dataType(),
		     HDF5ArrayPolicy<T,Array>::getRawPtr(data) );  
  }

  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Read in HDF5 file dataset attribute and save the data to an array
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in,out] data The data array that will be used to store the HDF5 file 
 * dataset attribute.
 * \param[in] location_in_file The location in the HDF5 file where the data will
 * be read from.
 * \param[in] attribute_name The name of the dataset attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::readArrayFromDataSetAttribute( Array<T> &data,
						     const std::string 
						       &dataset_location,
						     const std::string 
						       &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (dataset_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
  
  // HDF5 exceptions can be thrown when opening and reading from datasets
  try
  {
    H5::DataSet dataset(d_hdf5_file->openDataSet( dataset_location ) );

    // Get the attribute associated with the dataset
    H5::Attribute attribute(dataset.openAttribute( attribute_name) );
    
    // Get the dataspace of the attribute
    H5::DataSpace dataspace = attribute.getSpace();
    
    // Get the number of dimensions in the dataspace
    int rank = dataspace.getSimpleExtentNdims();
    
    // Get the dimension size of each dimension in the dataspace
    hsize_t dims[rank];
    int ndims = dataspace.getSimpleExtentDims( dims, NULL );
    
    // Resize the output array to the size of the dataspace
    typename Array<T>::size_type size = dims[0];
    for( unsigned int i = 1; i < rank; ++i )
      size *= dims[i];
    
    HDF5ArrayPolicy<T,Array>::resize( data, size );
    
    // Read the data in the dataset and save it to the output array
    attribute.read( HDF5TypeTraits<T>::dataType(),
		    HDF5ArrayPolicy<T,Array>::getRawPtr(data) );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Write attribute to HDF5 file dataset
/*! \tparam T Data type to be written.
 * \param[in] value The data value to write to the HDF5 file dataset attribute.
 * \param[in] dataset_location The location in the HDF5 file where the dataset
 * attribute will written.
 * \param[in] attribute_name The name of the dataset attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameter should never be given explicitly.
 *       The compiler will be able to deduce the template parameter based on 
 *       the value that is passed to the function.
 *  <li> The FACEMC::HDF5TypeTraits struct is critical to the generality of this
 *       function. Review this structs to better understand how this function 
 *       operates.
 * </ul>
 */
template<typename T>
void HDF5FileHandler::writeValueToDataSetAttribute( const T &value,
						    const std::string 
						      &dataset_location,
						    const std::string
						      &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (dataset_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
  
  // HDF5 exceptions can be thrown when opening a group, creating an attribute,
  // or writing an attribute to a group
  try
  {
    hsize_t dim = 1;
    H5::DataSpace space( 1, &dim );
    H5::DataSet dataset(d_hdf5_file->openDataSet( dataset_location ) ); 
    H5::Attribute attribute(dataset.createAttribute( 
					  attribute_name, 
					  HDF5TypeTraits<T>::dataType(),
					  space ) );
    
    attribute.write( HDF5TypeTraits<T>::dataType(),
		     &value );  
  }

  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Read in HDF5 file dataset attribute and save the single value
/*! \tparam T Data type to be read.
 * \param[in,out] value The data value that will store the HDF5 file dataset 
 * attribute.
 * \param[in] dataset_location The location in the HDF5 file where the dataset
 * attribute will written.
 * \param[in] attribute_name The name of the dataset attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameter should never be given explicitly.
 *       The compiler will be able to deduce the template parameter based on 
 *       the value that is passed to the function.
 *  <li> The FACEMC::HDF5TypeTraits struct is critical to the generality of this
 *       function. Review this structs to better understand how this function 
 *       operates.
 * </ul>
 */
template<typename T>
void HDF5FileHandler::readValueFromDataSetAttribute( T &value,
						     const std::string 
						       &dataset_location,
						     const std::string 
						       &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (dataset_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
  
  // HDF5 exceptions can be thrown when opening and reading from datasets
  try
  {
    H5::DataSet dataset(d_hdf5_file->openDataSet( dataset_location ) );

    // Get the attribute associated with the dataset
    H5::Attribute attribute(dataset.openAttribute( attribute_name) );
    
    // Get the dataspace of the attribute
    H5::DataSpace dataspace = attribute.getSpace();
    
    // Get the number of dimensions in the dataspace
    int rank = dataspace.getSimpleExtentNdims();
    
    // Get the dimension size of each dimension in the dataspace
    hsize_t dims[rank];
    int ndims = dataspace.getSimpleExtentDims( dims, NULL );

    FACEMC_ASSERT_ALWAYS( (rank == 1 && dims[0] == 1) );
    
    // Read the data in the dataset and save it to the output array
    attribute.read( HDF5TypeTraits<T>::dataType(),
		    &value );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Write attribute to HDF5 file group
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in] data The data array to write to the HDF5 file group attribute.
 * \param[in] group_location The location in the HDF5 file where the group
 * attribute will written.
 * \param[in] attribute_name The name of the group attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::writeArrayToGroupAttribute( const Array<T> &data,
						  const std::string 
						    &group_location,
						  const std::string
						    &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (group_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );

  // Create any parent groups that do not exist yet in the location path
  createParentGroups( group_location );
    
  // HDF5 exceptions can be thrown when opening a group, creating an attribute,
  // or writing an attribute to a group
  try
  {
    hsize_t dim = HDF5ArrayPolicy<T,Array>::size(data);
    H5::DataSpace space( 1, &dim );
    H5::Group group(d_hdf5_file->openGroup( group_location ) ); 
    H5::Attribute attribute(group.createAttribute( 
					  attribute_name, 
					  HDF5TypeTraits<T>::dataType(),
					  space ) );
    
    attribute.write( HDF5TypeTraits<T>::dataType(),
		     HDF5ArrayPolicy<T,Array>::getRawPtr(data) );  
  }

  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Read in HDF5 file group attribute and save the data to an array
/*! \tparam T Data type contained in array
 * \tparam Array An array class with a single template parameter. Any of the
 * Teuchos array classes will work. Because std::vector has two template 
 * parameters it cannot be used. The Teuchos array classes are all extensions
 * of the std::vector though.
 * \param[in,out] data The data array that will be used to store the HDF5 file 
 * group attribute.
 * \param[in] group_location The location in the HDF5 file where the group is
 * located.
 * \param[in] attribute_name The name of the group attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameters should never be given explicitly.
 *       The compiler will be able to deduce the template parameters based on 
 *       the array that is passed to the function.
 *  <li> The FACEMC::HDF5ArrayPolicy and FACEMC::HDF5TypeTraits structs are
 *       critical to the generality of this function. Review these structs to
 *       better understand how this function operates.
 *  <li> The template parameter T and Array template template parameter could
 *       be combined into a single template parameter Array. The type T could
 *       then be accessed using "typename Array::value_type". This would allow
 *       for std::vector to be used. The current method seems more verbose.
 * </ul>
 */
template<typename T, template<typename> class Array>
void HDF5FileHandler::readArrayFromGroupAttribute( Array<T> &data,
						   const std::string 
						     &group_location,
						   const std::string 
						     &attribute_name )
{
  // The group_location must be absolute (start with /)
  testPrecondition( (group_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
  
  // HDF5 exceptions can be thrown when opening and reading from datasets
  try
  {
    H5::Group group(d_hdf5_file->openGroup( group_location ) );

    // Get the attribute associated with the group
    H5::Attribute attribute(group.openAttribute( attribute_name) );
    
    // Get the dataspace of the attribute
    H5::DataSpace dataspace = attribute.getSpace();
    
    // Get the number of dimensions in the dataspace
    int rank = dataspace.getSimpleExtentNdims();
    
    // Get the dimension size of each dimension in the dataspace
    hsize_t dims[rank];
    int ndims = dataspace.getSimpleExtentDims( dims, NULL );
    
    // Resize the output array to the size of the dataspace
    typename Array<T>::size_type size = dims[0];
    for( unsigned int i = 1; i < rank; ++i )
      size *= dims[i];
    
    HDF5ArrayPolicy<T,Array>::resize( data, size );
    
    // Read the data in the dataset and save it to the output array
    attribute.read( HDF5TypeTraits<T>::dataType(),
		    HDF5ArrayPolicy<T,Array>::getRawPtr(data) );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Write attribute to HDF5 file group
/*! \tparam T Data type of value to write
 * \param[in] value The data value to write to the HDF5 file group attribute.
 * \param[in] group_location The location in the HDF5 file where the group
 * is located.
 * \param[in] attribute_name The name of the group attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameter should never be given explicitly.
 *       The compiler will be able to deduce the template parameter based on 
 *       the value that is passed to the function.
 *  <li> The FACEMC::HDF5TypeTraits struct is critical to the generality of this
 *       function. Review this structs to better understand how this function 
 *       operates.
 * </ul>
 */
template<typename T>
void HDF5FileHandler::writeValueToGroupAttribute( const T &value,
						  const std::string 
						    &group_location,
						  const std::string
						    &attribute_name )
{
  // The dataset_location must be absolute (start with /)
  testPrecondition( (group_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );

  // Create any parent groups that do not exist yet in the location path
  createParentGroups( group_location );
    
  // HDF5 exceptions can be thrown when opening a group, creating an attribute,
  // or writing an attribute to a group
  try
  {
    hsize_t dim = 1;
    H5::DataSpace space( 1, &dim );
    H5::Group group(d_hdf5_file->openGroup( group_location ) ); 
    H5::Attribute attribute(group.createAttribute( 
					  attribute_name, 
					  HDF5TypeTraits<T>::dataType(),
					  space ) );
    
    attribute.write( HDF5TypeTraits<T>::dataType(),
		     &value );  
  }

  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

// Read in HDF5 file group attribute and save the single value
/*! \tparam T Data type to be read.
 * \param[in,out] value The data value that will store the HDF5 file group 
 * attribute.
 * \param[in] group_location The location in the HDF5 file where the group
 * is located.
 * \param[in] attribute_name The name of the group attribute.
 * \pre 
 * <ul>
 *  <li> A valid location string, which is any string that does not start with
 *       a "/", must be given to this function.
 *  <li> A valid attribute name string, which is any string that does not
 *       contian a "/", must be given to this function.
 * </ul>
 * \note Developers: 
 * <ul>
 *  <li> The template parameter should never be given explicitly.
 *       The compiler will be able to deduce the template parameter based on 
 *       the value that is passed to the function.
 *  <li> The FACEMC::HDF5TypeTraits struct is critical to the generality of this
 *       function. Review this structs to better understand how this function 
 *       operates.
 * </ul>
 */
template<typename T>
void HDF5FileHandler::readValueFromGroupAttribute( T &value,
						   const std::string 
						     &group_location,
						   const std::string 
						     &attribute_name )
{
  // The group_location must be absolute (start with /)
  testPrecondition( (group_location.compare( 0, 1, "/" ) == 0) );
  // The attribute name can contain any character except /
  testPrecondition( (attribute_name.find( "/" ) == std::string::npos ) );
  
  // HDF5 exceptions can be thrown when opening and reading from datasets
  try
  {
    H5::Group group(d_hdf5_file->openGroup( group_location ) );

    // Get the attribute associated with the group
    H5::Attribute attribute(group.openAttribute( attribute_name) );
    
    // Get the dataspace of the attribute
    H5::DataSpace dataspace = attribute.getSpace();
    
    // Get the number of dimensions in the dataspace
    int rank = dataspace.getSimpleExtentNdims();
    
    // Get the dimension size of each dimension in the dataspace
    hsize_t dims[rank];
    int ndims = dataspace.getSimpleExtentDims( dims, NULL );
    
    FACEMC_ASSERT_ALWAYS_MSG( (rank == 1 && dims[0] == 1),
			      "Fatal Error: Cannot read a single value from an attribute array.");

    // Read the data in the dataset and save it to the output array
    attribute.read( HDF5TypeTraits<T>::dataType(),
		    &value );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

} // end FACEMC namespace

#endif // end HDF5_FILE_HANDLER_DEF_HPP

//---------------------------------------------------------------------------//
// end HDF5FileHandler_def.hpp
//---------------------------------------------------------------------------//
