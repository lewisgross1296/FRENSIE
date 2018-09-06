//---------------------------------------------------------------------------//
//!
//! \file   Data_ACEPhotoatomicDataProperties.hpp
//! \author Alex Robinson
//! \brief  The ACE photoatomic data properties class declaration
//!
//---------------------------------------------------------------------------//

#ifndef DATA_ACE_PHOTOATOMIC_DATA_PROPERTIES_HPP
#define DATA_ACE_PHOTOATOMIC_DATA_PROPERTIES_HPP

// FRENSIE Includes
#include "Data_PhotoatomicDataProperties.hpp"
#include "Data_ACETableName.hpp"
#include "Utility_ExplicitSerializationTemplateInstantiationMacros.hpp"

namespace Data{

//! The ACE photoatomic data properties class
class ACEPhotoatomicDataProperties : public PhotoatomicDataProperties
{

public:

  //! Constructor
  ACEPhotoatomicDataProperties( const AtomicWeight atomic_weight,
                                const boost::filesystem::path& file_path,
                                const size_t file_start_line,
                                const ACETableName& file_table_name );

  //! Destructor
  ~ACEPhotoatomicDataProperties()
  { /* ... */ }

  //! Get the atom that the file specifies data for
  AtomType atom() const override;

  //! Get the photoatomic data file type
  FileType fileType() const override;

  //! Get the atomic weight of the nuclide that the file specifies data for
  AtomicWeight atomicWeight() const override;

  //! Get the photoatomic data file path (relative to the data directory)
  boost::filesystem::path filePath() const override;

  //! Get the photoatomic data file start line
  size_t fileStartLine() const override;

  //! Get the photoatomic data file version
  unsigned fileVersion() const override;

  //! Get the photoatomic table name
  std::string tableName() const override;

  //! Clone the properties
  ACEPhotoatomicDataProperties* clone() const override;

private:

  // Default destructor
  ACEPhotoatomicDataProperties();

  // Copy constructor
  ACEPhotoatomicDataProperties( const ACEPhotoatomicDataProperties& other );

  // Assignment operator
  ACEPhotoatomicDataProperties& operator=( const ACEPhotoatomicDataProperties& other );

  // Save the properties to an archive
  template<typename Archive>
  void save( Archive& ar, const unsigned version ) const;

  // Load the properties from an archive
  template<typename Archive>
  void load( Archive& ar, const unsigned version );

  BOOST_SERIALIZATION_SPLIT_MEMBER();

  // Declare the boost serialization access object as a friend
  friend class boost::serialization::access;

  //  The atomic weight of the atom that the file specifies data for
  AtomicWeight d_atomic_weight;

  // The file path (relative to the data directory)
  boost::filesystem::path d_file_path;

  // The file start line
  size_t d_file_start_line;

  // The file table name
  ACETableName d_file_table_name;
};

// Save the properties to an archive
template<typename Archive>
void ACEPhotoatomicDataProperties::save( Archive& ar, const unsigned version ) const
{
  // Save the base class first
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( PhotoatomicDataProperties );

  // Save the local member data
  ar & BOOST_SERIALIZATION_NVP( d_atomic_weight );
  
  std::string raw_path = d_file_path.string();
  
  ar & BOOST_SERIALIZATION_NVP( raw_path );
  ar & BOOST_SERIALIZATION_NVP( d_file_start_line );
  ar & BOOST_SERIALIZATION_NVP( d_file_table_name );
}

// Load the properties from an archive
template<typename Archive>
void ACEPhotoatomicDataProperties::load( Archive& ar, const unsigned version )
{
  // Load the base class first
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( PhotoatomicDataProperties );

  // Load the local member data
  ar & BOOST_SERIALIZATION_NVP( d_atomic_weight );
  
  std::string raw_path;  
  ar & BOOST_SERIALIZATION_NVP( raw_path );

  d_file_path = raw_path;
  d_file_path.make_preferred();
  
  ar & BOOST_SERIALIZATION_NVP( d_file_start_line );
  ar & BOOST_SERIALIZATION_NVP( d_file_table_name );
}

} // end Data namespace

BOOST_SERIALIZATION_CLASS_VERSION( ACEPhotoatomicDataProperties, Data, 0 );
BOOST_SERIALIZATION_CLASS_EXPORT_STANDARD_KEY( ACEPhotoatomicDataProperties, Data );
EXTERN_EXPLICIT_CLASS_SAVE_LOAD_INST( Data, ACEPhotoatomicDataProperties );

#endif // end DATA_ACE_PHOTOATOMIC_DATA_PROPERTIES_HPP

//---------------------------------------------------------------------------//
// end Data_ACEPhotoatomicDataProperties.hpp
//---------------------------------------------------------------------------//
