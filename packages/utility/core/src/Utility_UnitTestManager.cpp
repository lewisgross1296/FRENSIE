//---------------------------------------------------------------------------//
//!
//! \file   Utility_UnitTestManager.cpp
//! \author Alex Robinson
//! \brief  The unit test manager class definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <list>

// FRENSIE Includes
#include "Utility_UnitTestManager.hpp"
#include "Utility_GlobalMPISession.hpp"
#include "FRENSIE_config.hpp"

//! Use this to indicate that logs on all processes should be added to the report
#define REPORT_ON_ALL_PROCS -1

namespace Utility{

//! The distributed unit test manager
class DistributedUnitTestManager : public UnitTestManager
{

protected:

  // Constructor
  DistributedUnitTestManager();

  //! Flush logs and add to report
  void flushLogsAndAddToReport( std::ostringstream& log,
                                const int proc ) override;

  //! Print the help message
  void printHelpMessage() override;

  //! Print the test details
  void printTestDetails() override;

  //! Print the sorting tests notification
  void printSortingTestsNotification() override;

  //! Print the running tests notification
  void printRunningTestsNotification() override;

  //! Print the unit test header
  void printUnitTestHeader( const size_t unit_test_id,
                            const UnitTest& unit_test ) override;

  //! Print the operation failed notification
  void printOperationFailedNotification() override;

  //! Print the operation passed notification
  void printOperationPassedNotification() override;

  //! Print the operation time
  void printOperationTime( const double time_in_sec,
                           const bool wrapped,
                           const bool goto_newline ) override;

  //! Print the operation location
  void printOperationLocation( const std::string& file_name,
                               const size_t line_number ) override;

  //! Print the operation log
  void printOperationLog( const bool local_success,
                          const std::string& log_header,
                          std::ostringstream& log ) override;

  //! Print failed test summary header
  void printFailedTestSummaryHeader( const std::string& header_end ) override;

  //! Print failed test name
  void printFailedTestName(const std::string& global_failed_test_name,
                           const std::string& test_details,
                           const std::set<std::string>& local_failed_tests_set,
                           const bool goto_newline ) override;

  //! Print the unit test stats
  void printUnitTestStats( const std::string& summary_header ) override;

  //! Print the program execution time header
  void printProgramExecutionTimeHeader(
                                const double program_execution_time ) override;
};

//! The UnitTestManager private data class
class UnitTestManager::Data
{
  
public:

  //! The report level
  enum ReportLevel{ Detailed = 2, Basic = 1, Nothing = 0, Auto = -1 };

  //! The unit tests type
  typedef std::list<UnitTest*> UnitTests;

  //! Get the report levels
  static std::string getReportLevels();

  //! Constructor
  Data();

  //! Destructor
  ~Data();

  //! Return the unit test manager initializer
  UnitTestManager::Initializer& getInitializer();

  //! Set the unit test manager initializer
  void setInitializer( UnitTestManager::Initializer& initializer );

  //! Return the unit tests
  UnitTests& getUnitTests();

  //! Return the unit tests
  const UnitTests& getUnitTests() const;

  //! Return the custom command line options
  boost::program_options::options_description& getCustomCommandLineOptions();

  //! Return the custom command line options
  const boost::program_options::options_description& getCustomCommandLineOptions() const;

  //! Parse command line options
  void parseCommandLineOptions( int argc, char** argv );

  //! Redirect std::cout and std::cerr to the string stream
  void redirectStdOutput( std::ostringstring& oss ) const;

  //! Restore std::cout and std::cerr
  void restoreStdOutput() const;

  //! Get the command line option
  boost::program_options::variable_value
  getCommandLineOptionValue( const std::string& option_name ) const;

  //! Check if a help message was requested
  bool helpMessageRequested() const;

  //! Print the help message
  void printHelpMessage();

  //! Check if a group list was requested
  bool groupListRequested() const;

  //! Print the group list
  void printGroupList();

  //! Check if a test list was requested
  bool testListRequested() const;

  //! Print the test list
  void printTestList();

  //! Check if a data list was requested
  bool dataListRequested() const;

  //! Print the data list
  void printDataList() const;

  //! Get the report level
  ReportLevel getReportLevel() const;
  
  //! Get the report sink The report sink
  std::ostream& getReportSink();

  //! Get the group filter
  const std::regex& getGroupFilter() const;

  //! Get the test filter
  const std::regex& getTestFilter() const;

  //! Get the data filter
  const std::regex& getDataFilter() const;

  //! Get the number of benchmark samples
  size_t getNumberOfBenchmarkSamples() const;

  //! Return the run test counter
  size_t& getRunTestCounter();

  //! Return the run test counter
  const size_t& getRunTestCounter() const;

  //! Return the passed test counter
  size_t& getPassedTestCounter();

  //! Return the passed test counter
  const size_t& getPassedTestCounter() const;

  //! Return the total unit test execution time
  double& getTotalUnitTestExecutionTime();

  //! Return the total unit test execution time
  const double& getTotalUnitTestExecutionTime() const;

private:

  // The unit tests
  UnitTests d_unit_tests;

  // The command line options
  boost::program_options::options_description d_command_line_options;

  // The custom command line options
  boost::program_options::options_description d_custom_command_line_options;

  // The command line arguments
  boost::program_options::variables_map d_command_line_arguments;

  // The unit test manager initializer
  UnitTestManager::Initializer* d_initializer;

  // The report level
  ReportLevel d_report_level;
  
  // The report sink
  std::unique_ptr<std::ostream> d_report_sink;

  // The report sink buffer
  std::streambuf* d_report_sink_buffer;

  // The std::cout buffer
  std::streambuf* d_stdout_buffer;

  // The std::cerr buffer
  std::streambuf* d_stderr_buffer;

  // The group filter
  std::regex d_group_filter;

  // The test filter
  std::regex d_test_filter;

  // The data filter
  std::regex d_data_filter;

  // The number of benchmark samples
  size_t d_number_of_benchmark_samples;

  // The run test counter
  size_t d_run_test_counter;

  // The passed test counter
  size_t d_passed_test_counter;

  // The total unit test execution time (s)
  double d_total_unit_test_execution_time;
};

//! Less than comparison operator for UnitTest*
inline bool operator<( const UnitTest* lhs, const UnitTest* rhs )
{
  return *lhs < *rhs;
}

/*! Output stream operator for UnitTestManager::Data::ReportLevel
 *
 * Required by boost::program_options.
 */
std::ostream& operator<<( std::ostream& os,
                          UnitTestManager::Data::ReportLevel report_level )
{
  switch( report_level )
  {
    case UnitTestManager::Data::Detailed:
    {
      os << "detailed";
      return os;
    }
    case UnitTestManager::Data::Basic:
    {
      os << "basic";
      return os;
    }
    case UnitTestManager::Data::Nothing:
    {
      os << "nothing";
      return os;
    }
    case UnitTestManager::Data::Auto:
    {
      os << "auto";
      return os;
    }
    default:
    {
      os << "auto";
      return os;
    }
  }
}

// Get the report levels
std::string UnitTestManager::Data::getReportLevels()
{
  std::ostringstream oss;
  
  oss << UnitTestManager::Data::Detailed << " "
      << UnitTestManager::Data::Basic << " "
      << UnitTestManager::Data::Nothing << " "
      << UnitTestManager::Data::Auto;

  return oss.str();
}

// Constructor
UnitTestManager::Data::Data()
  : d_unit_tests(),
    d_command_line_options(),
    d_custom_command_line_options(),
    d_command_line_arguments(),
    d_initializer( NULL ),
    d_report_level( Data::Auto ),
    d_report_sink( &std::cout, [](std::ostream*){}),
    d_report_sink_buffer( NULL ),
    d_stdout_buffer( NULL ),
    d_stderr_buffer( NULL ),
    d_group_filter(),
    d_test_filter(),
    d_data_filter(),
    d_number_of_benchmark_samples( 1 ),
    d_run_test_counter( 0 ),
    d_passed_test_counter( 0 ),
    d_total_unit_test_execution_time( 0.0 )
{
  // Initialize the command line options
  d_command_line_options.add_options()
    ("help,h", "produce help message")
    ("show_groups,g",
     boost::program_options::value<bool>()->default_value( false ),
     "show the group names and exit")
    ("show_tests,t",
     boost::program_options::value<bool>()->default_value( false ),
     "show the test names and exit")
    ("show_data,d",
     boost::program_options::value<bool>()->default_value( false ),
     "show the test data tables and exit")
    ("report_level,r",
     boost::program_options::value<Data::ReportLevel>()->default_value( d_report_level ),
     "set the report level (" + this->getReportLevels().c_str() + ")")
    ("report_sink,o",
     boost::program_options::value<std::string>()->default_value( "stdout" ),
     "set the test report sink (stdout, stderr or file name)")
    ("group_filter",
     boost::program_options::value<std::string>()->default_value( ".*" ),
     "only group names that match the regex will be executed")
    ("test_filter",
     boost::program_options::value<std::string>()->default_value( ".*" ),
     "only test names that match the regex will be executed")
    ("data_filter",
     boost::program_options::value<std::string>()->default_value( ".*" ),
     "only data table row names that match the regex will be executed")
    ("benchmark_samples,b",
     boost::program_options::value<size_t>()->default_value( d_number_of_benchmark_samples ),
     "set the number of times to run each benchmark (the reported time will"
     "be the average of all samples)");
}

// Destructor
UnitTestManager::Data::~Data()
{
  // Restore the sink buffer if it was extracted
  if( d_report_sink_buffer )
    d_report_sink->rdbuf( d_report_sink_buffer );
}

// Return the unit test manager initializer
UnitTestManager::Initializer& UnitTestManager::Data::getInitializer()
{
  if( d_initializer )
    return *d_initializer;
  else
  {
    static UnitTestManager::Initializer default_intializer;
    
    return default_initializer;
  }
}

// Set the unit test manager initializer
void UnitTestManager::Data::setInitializer(
                                    UnitTestManager::Initializer& initializer )
{
  d_initializer = &initializer;
}

// Return the unit tests
UnitTests& UnitTestManager::Data::getUnitTests()
{
  return d_unit_tests;
}

// Return the unit tests
const UnitTests& UnitTestManager::Data::getUnitTests() const
{
  return d_unit_tests;
}

// Return the custom command line options
boost::program_options::options_description&
UnitTestManager::Data::getCustomCommandLineOptions()
{
  return d_custom_command_line_options;
}

// Return the custom command line options
const boost::program_options::options_description&
UnitTestManager::Data::getCustomCommandLineOptions() const
{
  return d_custom_command_line_options;
}

// Parse command line options
void UnitTestManager::Data::parseCommandLineOptions( int argc, char** argv )
{
  // Merge the command line options with the custom command line options
  boost::program_options::options_description
    cmdline_options( "Allowed options" );
  
  cmdline_options.add( d_command_line_options );
  cmdline_options.add( d_custom_command_line_options );

  // Parse the command line arguments
  boost::program_options::store(
       boost::program_options::command_line_parser(argc, argv).options(cmdline_options).run(),
       d_command_line_arguments() );

  boost::program_options::notify( d_command_line_arguments() );

  // Extract the report level
  d_report_level =
    d_command_line_arguments["report_level"].as<Data::ReportLevel>();

  // Extract the report sink name and create the sink
  std::string report_sink_name =
    d_command_line_arguments["report_sink"].as<std::string>();

  boost::algorithm::to_lower(report_sink_name);

  if( report_sink_name == "stdout" )
    d_report_sink.reset( &std::cout, [](std::ostream*){} );
  else if( report_sink_name == "stderr" )
    d_report_sink.reset( &std::cerr, [](std::ostream*){} );
  else
  {
    d_report_sink.reset( new std::ofstream file_sink(
                 d_command_line_arguments["report_sink"].as<std::string>() ) );
  }
  
  // Suppress all output
  if( d_report_level == Data::Nothing )
  {
    if( d_report_sink->rdbuf() )
      d_report_sink_buffer = d_report_sink->rdbuf();

    d_report_level->rdbuf( NULL );
  }

  // Extract the filters
  d_group_filter = d_command_line_arguments["group_filter"].as<std::string>();
  d_test_filter = d_command_line_arguments["test_filter"].as<std::string>();
  d_data_filter = d_command_line_arguments["data_filter"].as<std::string>();

  // Extract the number of benchmark samples
  d_number_of_benchmark_samples =
    d_command_line_arguments["benchmark_samples"].as<size_t>();
}

// Redirect std::cout and std::cerr to the string stream
void UnitTestManager::Data::redirectStdOutput( std::ostringstring& oss ) const
{
  d_stdout_buffer = std::cout.rdbuf();
  std::cout.rdbuf( oss.rdbuf() );

  d_stdcerr_buffer = std::cerr.rdbuf();
  std::cerr.rdbuf( oss.rdbuf() );
}

// Restore std::cout and std::cerr
void UnitTestManager::Data::restoreStdOutput() const
{
  std::cout.rdbuf( d_stdout_buffer );
  std::cerr.rdbuf( d_stderr_buffer );
}

// Get the command line option
boost::program_options::variable_value
UnitTestManager::Data::getCommandLineOptionValue(
                                         const std::string& option_name ) const
{
  if( d_command_line_arguments.count( option_name ) )
    return d_command_line_arguments[option_name].as<T>();
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Command line option " << option_name <<
                     " was not used!" );
  }
}

// Check if a help message was requested
bool UnitTestManager::Data::helpMessageRequested() const
{
  if( d_command_line_arguments.count( "help" ) )
    return true;
  else
    return false;
}

// Print the help message
bool UnitTestManager::Data::printHelpMessage()
{
  boost::program_options::options_description
    cmdline_options( "Allowed options" );
  
  cmdline_options.add( d_command_line_options );
  cmdline_options.add( d_custom_command_line_options );
  
  this->getReportSink() << cmdline_options << std::endl;
}

// Check if a group list was requested
bool UnitTestManager::Data::groupListRequested() const
{
  if( d_command_line_arguments.count( "show_groups" ) )
    return true;
  else
    return false;
}

// Print the group list
void UnitTestManager::Data::printGroupList()
{
  std::set<std::string> group_list;

  for( UnitTests::const_iterator it = d_unit_tests.begin();
       it != d_unit_tests.end();
       ++it )
  {
    group_list.insert( (*it)->getGroupName() );
  }

  if( group_list.size() > 0 )
  {
    this->getReportSink() << "Groups: " << std::endl;
    
    for( std::set<std::string>::const_iterator it = group_list.begin();
         it != group_list.end();
         ++it )
    {
      this->getReportSink() << "  " << *it << std::endl;
    }
  }
  else
    this->getReportSink() << "No groups present!" << std::endl;
}

// Check if a test list was requested
bool UnitTestManager::Data::testListRequested() const
{
  if( d_command_line_arguments.count( "show_tests" ) )
    return true;
  else
    return false;
}

// Print the test list
void UnitTestManager::Data::printTestList()
{
  if( d_unit_tests.size() > 0 )
  {
    // Sort the tests
    d_unit_tests.sort();

    std::string current_group_name( "" );

    for( UnitTests::const_iterator it = d_unit_tests.begin();
       it != d_unit_tests.end();
       ++it )
    {
      if( current_group_name != (*it)->getGroupName() )
      {
        current_group_name = (*it)->getGroupName();
        
        this->getReportSink() << "\nTests for group " << current_group_name
                              << ": " << std::endl;
      }
      
      this->getReportSink() << "  " << (*it)->getTestName() << std::endl;
    }
  }
  else
    this->getReportSink() << "No tests present!" << std::endl;
}

// Check if a data list was requested
bool UnitTestManager::Data::dataListRequested() const
{
  if( d_command_line_arguments.count( "show_data" ) )
    return true;
  else
    return false;
}

// Print the data list
void UnitTestManager::Data::printDataList() const
{
  if( d_unit_tests.size() > 0 )
  {
    // Sort the tests
    d_unit_tests.sort();

    std::string current_combined_name( "" );

    for( UnitTests::const_iterator it = d_unit_tests.begin();
       it != d_unit_tests.end();
       ++it )
    {
      std::string combined_name = (*it)->getCombinedGroupTestName();
      
      if( current_combined_name != combined_name )
      {
        current_combined_name = combined_name;

        if( (*it)->getDataName().size() > 0 )
        {
          this->getReportSink() << "\nData for test " << current_combined_name
                                << ": " << std::endl;
        }
      }

      if( (*it)->getDataName().size() > 0 )
        this->getReportSink() << "  " << (*it)->getDataName() << std::endl;
    }
  }
  else
    this->getReportSink() << "No data present!" << std::endl;
}
  
// Get the report level
UnitTestManager::Data::ReportLevel UnitTestManager::Data::getReportLevel() const
{
  return d_report_level;
}
  
// Get the report sink The report sink
std::ostream& UnitTestManager::Data::getReportSink()
{
  return *d_report_sink;
}

// Get the group filter
const std::regex& UnitTestManager::Data::getGroupFilter() const
{
  return d_group_filter;
}

// Get the test filter
const std::regex& UnitTestManager::Data::getTestFilter() const
{
  return d_test_filter;
}

// Get the data filter
const std::regex& UnitTestManager::Data::getDataFilter() const
{
  return d_data_filter;
}

// Get the number of benchmark samples
size_t UnitTestManager::Data::getNumberOfBenchmarkSamples() const
{
  return d_number_of_benchmark_samples;
}
  
// Return the run test counter
size_t& UnitTestManager::Data::getRunTestCounter()
{
  return d_run_test_counter;
}

// Return the run test counter
/*! \details A constant reference is returned so that any attempt to 
 * increment the return directly will result in a compilation error.
 */
const size_t& UnitTestManager::Data::getRunTestCounter() const
{
  return d_run_test_counter;
}

// Return the passed test counter
size_t& UnitTestManager::Data::getPassedTestCounter()
{
  return d_passed_test_counter;
}

// Return the passed test counter
/*! \details A constant reference is returned so that any attempt to 
 * increment the return directly will result in a compilation error.
 */
const size_t& UnitTestManager::Data::getPassedTestCounter() const
{
  return d_passed_test_counter;
}

// Return the total unit test execution time
double& UnitTestManager::Data::getTotalUnitTestExecutionTime()
{
  return d_total_unit_test_execution_time;
}

// Return the total unit test execution time
/*! \details A constant reference is returned so that any attempt to 
 * increment the return directly will result in a compilation error.
 */
const double& UnitTestManager::Data::getTotalUnitTestExecutionTime() const
{
  return d_total_unit_test_execution_time;
}

// Initialize static member data
std::unique_ptr<UnitTestManager> UnitTestManager::s_instance;

// Get the unit test manager instance
UnitTestManager& UnitTestManager::getInstance()
{
  if( !s_instance )
  {
    s_instance.reset( [](UnitTestManager*){
#ifdef HAVE_FRENSIE_MPI
                         return new DistributedUnitTestManager;
#else
                         return new UnitTestManager;
                      } );
  }
      
  return *s_instance;
}

// Constructor
UnitTestManager::UnitTestManager()
  : d_data( new UnitTestManager::Data )
{ /* ... */ }

// Constructor
DistributedUnitTestManager::DistributedUnitTestManager()
  : UnitTestManager()
{ /* ... */ }

// Set the initializer
/*! \details This method should never be called directly (only through the
 * test framework macros)
 */
void UnitTestManager::setInitializer( UnitTestManager::Initializer& initializer )
{
  d_data->setInitializer( initializer );
}

// Add a unit test
/*! \details This method should never be called directly (only through the
 * test framework macros)
 */
void UnitTestManager::addUnitTest( UnitTest& test )
{
  d_data->getUnitTests().push_back( &test );
}

// Parse command-line options and run registered unit tests
/*! \details This is all that needs be called from the main function (return
 * the value that is returned from this method).
 */
int UnitTestManager::runUnitTests( int* argc, char*** argv ) const
{
  Utility::GlobalMPISession& mpi_session =
    Utility::GlobalMPISession::initialize( argc, argv, &d_data->getReportSink() );
  
  const double program_start_time = mpi_session.getWallTime();

  // Set up the logs - we will use an ostringstream as the log sink so that
  // we can intercept log entries and format them before passing them to
  // the actual test sink
  std::ostringstream log;
  
  {
    boost::shared_ptr<std::ostream> log_stream( &log, boost::null_deleter() );

    FRENSIE_ADD_STANDARD_LOG_ATTRIBUTES();
    FRENSIE_SETUP_STANDARD_SYNCHRONOUS_LOGS( log_stream );
  }

  // Initialize the unit test manager
  bool local_success = true;

  // All output will be redirected to the log
  d_data->redirectStdOutput( log );

  const double init_start_time = mpi_session.getWallTime();
  
  try{
    d_data->getInitializer().initializeUnitTestManager( *argc, *argv );
  }
  FRENSIE_TEST_CATCH_STATEMENTS( log, true, local_success );

  const double init_time = mpi_session.getWallTime() - init_start_time;

  d_data->restoreStdOutput();
  
  // Make sure that every node initialized successfully
  bool global_success = mpi_session.isGloballyTrue( local_success );
    
  this->summarizeInitializationResults( local_success, global_success, init_time );

  // Initialization was globally successful - proceed to unit test execution
  if( global_success )
  {
    // Help message requested
    if( d_data->helpMessageRequested() )
    {
      this->printHelpMessage();
    }
    // Test details requested
    else if( d_data->groupListRequested() ||
             d_data->testListRequested() ||
             d_data->dataListRequsted() )
    {
      this->printTestDetails();
    }
    // Run unit tests
    else
    {
      local_success = this->runUnitTests( log );
      
      global_success = mpi_session.isGloballyTrue( local_success );

      const double program_execution_time =
        mpi_session.getWallTime() - program_start_time;
      
      this->summarizeTestStats( program_execution_time,
                                local_success,
                                global_success );
    }
  }

  // Summarize the test results
  return this->summarizeResults( local_success, global_success );
}

// Flush logs and add to report
void UnitTestManager::flushLogsAndAddToReport( std::ostringstream& log,
                                               const int )
{
  // This will flush all pending log entries to the global log
  FRENSIE_FLUSH_ALL_LOGS();

  if( log.str().size() > 0 )
  {
    Utility::DynamicOutputFormatter formatter( log.str() );
    formatter.formatUnitTestKeywords();
    
    d_data->getReportSink() << formatter << std::endl;

    log.str( "" );
    log.clear();
  }
}

// Flush logs and add to report
void DistributedUnitTestManager::flushLogsAndAddToReport(
                                                       std::ostringstream& log,
                                                       const int proc )
{
  // This will flush all pending log entries to the global log
  if( proc == REPORT_ON_ALL_PROCS )
  {
    for( size_t i = 0; i < d_data->getMPISession().getNProc(); ++i )
    {
      if( i == d_data->getMPISession().getRank() )
      {
        if( log.str().size() > 0 )
        {
          d_data->getReportSink() << "proc " << i << " details:\n";

          UnitTestManager::flushLogsAndAddToReport( log, proc );
        }
      }
      
      Utility::GlobalMPISession::barrier();
    }
  }
  else
  {
    if( Utility::GlobalMPISession::getRank() == proc )
      UnitTestManager::flushLogsAndAddToReport( log, proc );
  }
}

// Print the help message
void UnitTestManager::printHelpMessage()
{
  d_data->printHelpMessage();
}

// Print the help message
void DistributedUnitTestManager::printHelpMessage()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printHelpMessage();

  Utility::GlobalMPISession::getRank().barrier();
}

// Print the test details
void UnitTestManager::printTestDetails()
{
  if( d_data->groupListRequested() )
    d_data->printGroupList();

  if( d_data->testListRequested() )
    d_data->printTestList();
  
  if( d_data->dataListRequested() )
    d_data->printDataList();
}

// Print the test details
void DistributedUnitTestManager::printTestDetails()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printTestDetails();

  Utility::GlobalMPISession::barrier();
}

// Run the registered unit tests
bool UnitTestManager::runUnitTests( std::ostringstream& log )
{
  bool local_success = true;
  
  std::vector<std::string> global_failed_tests;
  std::set<size_t> local_failed_tests_set;

  try{
    this->printSortingTestsNotification();

    d_data->getUnitTests().sort();

    this->printRunningTestsNotification();

    for( Data::UnitTests::const_iterator test_it = d_data->getUnitTests().begin();
         test_it != d_data->getUnitTests().end();
         ++test_it )
    {
      Utility::UnitTests& unit_test = *(*test_it);
      
      if( this->shouldUnitTestBeRun( unit_test ) )
      {
        // Increment the run test counter
        ++(d_data->getRunTestCounter());

        // Print the unit tests header
        this->printUnitTestHeader( d_data->getRunTestCounter(), unit_test );

        // Run the test
        const double test_start_time =
          Utility::GlobalMPISession::getWallTime();
        
        const bool local_test_success =
          this->runUnitTest( unit_test, log );

        const double test_time =
          Utility::GlobalMPISession::getWallTime() - test_start_time;

        // Update test stats
        if( local_test_success )
          ++(d_data->getPassedTestCounter())

        d_data->getTotalUnitTestExecutionTime() += test_time;

        // Report the test results
        const bool global_test_success =
          Utility::GlobalMPISession::isGloballyTrue( local_test_success );
        
        this->reportTestResult( unit_test,
                                log,
                                test_time,
                                local_test_success,
                                global_test_success,
                                global_failed_tests,
                                local_failed_tests_set );

        if( !local_test_success )
          local_success = false;
      }
    }
  }
  FRENSIE_TEST_CATCH_STATEMENTS( log, true, local_success );

  this->flushLogsAndAddToReport( log, REPORT_ON_ALL_PROCS );

  // If there are any failed tests provide a summary of them now
  this->summarizeFailedTests( global_failed_tests, local_failed_tests_set );

  return local_success;
}

// Print the sorting tests notification
void UnitTestManager::printSortingTestsNotification()
{
  d_data->getReportSink() << "\nSorting tests by group name and then "
                          << "by definition order ..."
                          << std::endl;
}

// Print the sorting tests notification
void DistributedUnitTestManager::printSortingTestsNotification()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printSortingTestsNotification();

  Utility::GlobalMPISession::barrier();
}

// Print the running tests notification
void UnitTestManager::printRunningTestsNotification()
{
  d_data->getReportSink() << "Running unit tests ...\n" << std::endl;
}

// Print the running tests notification
void DistributedUnitTestManager::printRunningTestsNotification()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printRunningTestsNotification();

  Utility::GlobalMPISession::barrier();
}

// Print the unit test header
void UnitTestManager::printUnitTestHeader( const size_t unit_test_id,
                                           const UnitTest& unit_test )
{
  d_data->getReportSink() << unit_test_id << ". "
                          << unit_test.getFullName() " ... ";
}

// Print the unit test header
void DistributedUnitTestManager::printUnitTestHeader(
                                                    const size_t unit_test_id,
                                                    const UnitTest& unit_test )
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printUnitTestHeader( unit_test_id, unit_test );

  Utility::GlobalMPISession::barrier();
}

// Check if the unit test should be run
bool UnitTestManager::shouldUnitTestBeRun( const UnitTest& unit_test )
{
  if( std::regex_search(unit_test.getGroupName(), d_data->getGroupFilter()) )
  {
    if( std::regex_search(unit_test.getTestName(), d_data->getTestFilter()) )
    {
      if( unit_test.getDataName() == 0 ||
          std::regex_search(unit_test.getDataName(), d_data->getDataFilter()) )
      {
        return true;
      }
    }
  }

  return false;
}

// Check if the test details should be reported
bool UnitTestManager::shouldUnitTestDetailsBeReported(
                                                     const bool success ) const
{
  return d_data->getReportLevel() == Data::Detailed ||
    (!success && d_data->getReportLevel() == Data::Auto)
}

// Run a single unit tests
bool UnitTestManager::runUnitTest( const UnitTest& unit_test,
                                   std::ostringstream& log )
{
  // All output will be redirected to the log
  d_data->redirectStdOutput( log );

  const bool result = unit_test.run( log );
  
  // Flush all logs
  FRENSIE_FLUSH_ALL_LOGS();

  // Restore the output streams
  d_data->restoreStdOutput();

  return result;
}

// Report the test result
void UnitTestManager::reportTestResult(
                                 const UnitTest& unit_test,
                                 const std::oststringstream& log,
                                 const double test_run_time,
                                 const bool local_success,
                                 const bool global_success,
                                 std::vector<std::string>& global_failed_tests,
                                 std::set<size_t>& local_failed_test_set )
{
  // Record the tests that failed on this processes
  if( !local_success )
    local_failed_tests_set.insert( unit_test.getFullName() );
  
  // Report the result of the test
  if( !global_success )
  {
    global_failed_tests.push_back( unit_test.getFullName() );

    this->printOperationFailedNofication();
  }
  else
    this->printOperationPassedNotification();

  // Report the time for each process
  this->printOpertionTime( test_run_time, true );
  
  // Report the test details
  if( this->shouldUnitTestDetailsBeReported( global_success ) )
  {
    this->printOperationLocation( unit_test.getFile(), unit_test.getLineNumber() );
    this->printOperationLog( local_success, "", log );
  }
}

// Print the operation failed notification
void UnitTestManager::printOperationFailedNotification()
{
  d_data->getReportSink() << "[" << Utility::Red("FAILED") << "] ";
}

// Print the operation failed notification
void DistributedUnitTestManager::printOperationFailedNotification()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printOperationPassedNotification();

  Utility::GlobalMPISession::barrier();
}

// Print the operation passed notification
void UnitTestManager::printOperationPassedNotification()
{
  d_data->getReportSink() << "[" << Utility::Green("Passed") << "] ";
}

// Print the operation passed notification
void DistributedUnitTestManager::printOperationPassedNotification()
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printOperationPassedNotification();

  Utility::GlobalMPISession::barrier();
}

// Print the operation time
void UnitTestManager::printOperationTime( const double time_in_sec,
                                          const bool wrapped,
                                          const bool goto_newline )
{
  d_data->getReportSink().setPrecision( 6 );

  if( wrapped )
    d_data->getReportSink() << "(";

  d_data->getReportSink() << test_time << " sec";

  if( wrapped )
    d_data->getReportSink() << ")";

  if( goto_newline )
    d_data->getReportSink() << std::endl;
}
  
// Print the operation time
void DistributedUnitTestManager::printOperationTime( const double time_in_sec,
                                                     const bool wrapped,
                                                     const bool goto_newline )
{
  for( int i = 0; i < Utility::GlobalMPISession::getSize(); ++i )
  {
    if( Utility::GlobalMPISession::getRank() == i )
    {
      if( i < Utility::GlobalMPISession::getSize() - 1 )
        UnitTestManager::printOperationTime( time_in_sec, wrapped, false );
      else
        UnitTestManager::printOperationTime( time_in_sec, wrapped, goto_newline );
    }
      
    Utility::GlobalMPISession::barrier();
  }
}

// Print the operation location
void UnitTestManager::printOperationLocation( const std::string& file_name,
                                              const size_t line_number )
{
  std::ostringstream oss;
  oss << file_name << ":" << line_number;
  
  d_data->getReportSink() << "  " << Utility::BoldCyan("Location:") << " "
                          << Utility::Bold(oss.str())
                          << std::endl;
}

// Print the operation location
void DetailedUnitTestManager::printOperationLocation(
                                                  const std::string& file_name,
                                                  const size_t line_number )
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printOperationLocation( file_name, line_number );

  Utility::GlobalMPISession::barrier();
}

// Print the operation log
void UnitTestManager::printOperationLog( const bool local_success,
                                         const std::string& log_header,
                                         std::ostringstream& log )
{
  // Add the operation log to the report
  if( log_header.size() > 0 )
  {
    d_data->getReportSink() << log_header << " ";

    if( local_success )
      UnitTestManager::printOperationPassedNotification();
    else
      UnitTestManager::printOperationFailedNotification();

    d_data->getReportSink() << std::endl;
  }
  
  UnitTestManager::flushLogsAndAddToReport( log, Utility::GlobalMPISession::getRank() );
}

// Print the operation log
void DistributedUnitTestManager::printOperationLog( const bool local_success,
                                                    const std::string&,
                                                    std::ostringstream& log )
{
  if( Utility::GlobalMPISession::getSize() == 1 )
    UnitTestManager::printOperationLog( local_success, "", log );
  else
  {
    for( size_t i = 0; i < Teuchos::GlobalMPISession::getNProcs(); ++i )
    {
      if( Utility::GlobalMPISession::getRank() == i )
      {
        if( this->shouldUnitTestDetailsBeReported( local_success ) )
        {
          std::ostringstream oss;
          oss << "  Proc " << i;

          UnitTestManager::printOperationLog( local_success, oss.str(), log );
        }
        else
        {
          // Clear the log
          log.str( "" );
          log.clear();
        }
      }

      Utility::GlobalMPISession::barrier();
    }
  }
}

// Summarize failed tests
void UnitTestManager::summarizeFailedTests(
                          const std::vector<std::string>& global_failed_tests,
                          const std::set<std::string>& local_failed_tests_set )
{
  if( global_failed_tests.size() > 0 )
  {
    this->printFailedTestSummaryHeader( "" );
    
    for( size_t i = 0; i < failed_tests.size(); ++i )
      this->printFailedTestName( test_name, "", local_failed_tests_set, true );
  }
}

// Print failed test summary header
void UnitTestManager::printFailedTestSummaryHeader(
                                                const std::string& header_end )
{
  d_data->getReportSink() << "\nThe following tests "
                          << Utility::Red("FAILED");

  if( header_end.size() > 0 )
    d_data->getReportSink() << " " << header_end;

  d_data->getReportSink() << ":" << std::endl;
}

// Print failed test summary header
void DistributedUnitTestManager::printFailedTestSummaryHeader(
                                                           const std::string& )
{
  if( Utility::GlobalMPISession::getRank() == 0 )
  {
    if( Utility::GlobalMPISession::getSize() == 1 )
      UnitTestManager::printFailedTestSummaryHeader( "" );
    else
      UnitTestManager::printFailedTestSummaryHeader( "on at least one proc" );
  }

  d_data->getMPISession().barrier();
}

// Print failed test name
void UnitTestManager::printFailedTestName(
                                const std::string& global_failed_test_name,
                                const std::string& test_details,
                                const std::set<std::string>&,
                                const bool goto_newline )
{
  d_data->getReportSink() << global_failed_test_name;

  if( test_details.size() > 0 )
    d_data->getReportSink() << test_details;

  if( goto_newline )
    d_data->getReportSink() << std::endl;
}

// Print failed test name
void DistributedUnitTestManager::printFailedTestName(
                           const std::string& global_failed_test_name,
                           const std::string&,
                           const std::set<std::string>& local_failed_tests_set,
                           const bool goto_newline )
{
  if( Utility::GlobalMPISession::getSize() == 1 )
  {
    UnitTestManager::printFailedTestName( global_failed_test_name,
                                          "",
                                          local_failed_tests_set,
                                          goto_newline );
  }
  else
  {
    const bool report_line_started = false;
    
    for( int i = 0; i < Utility::GlobalMPISession::getSize(); ++i )
    {
      if( Utility::GlobalMPISession::getRank() == i )
      {
        if( local_failed_tests_set.count( global_failed_test_name ) )
        {
          std::ostringstream oss;

          if( !report_line_started )
            oss << " (";
          else
            oss << ", ";
        
          oss << i;

          if( !report_line_started )
          {
            UnitTestManager::printFailedTestName( global_failed_test_name,
                                                  oss.str(),
                                                  local_failed_tests_set,
                                                  false );
            report_line_started = true;
          }
          else
          {
            UnitTestManager::printFailedTestName( "",
                                                  oss.str(),
                                                  local_failed_tests_set,
                                                  false );
          }
        }
      }
            
      Utility::GlobalMPISession::barrier();
    }

    if( Utility::GlobalMPISession::getRank() == 0 )
    {
      UnitTestManager::printFailedTestName( "",
                                            "",
                                            local_failed_tests_set,
                                            goto_newline );
    }

    Utility::GlobalMPISession::barrier();
  }
}

// Summarize the initialization results
void UnitTestManager::summarizeInitializationResults(
                                              std::ostringstream& log,
                                              const double initialization_time,
                                              const bool local_success,
                                              const bool global_success )
{
  d_data->getReportSink() << "Initialization ";

  // Report the results of the initialization
  if( global_success )
    d_data->getReportSink() << "completed";
  else
    d_data->getReportSink() << Utility::Red("Failed");

  // Report the initialization time for each process
  this->printOperationTime( initialization_time, true );

  // Report the initialization details
  if( this->shouldUnitTestDetailsBeReported( global_success ) )
    this->printOperationLog( local_success, "", log );
}

// Summarize the test results
void UnitTestManager::summarizeTestStats( const double program_execution_time,
                                          const bool local_success,
                                          const bool global_success )
{
  this->printUnitTestStats( "Test Statistics Summary" );
  this->printProgramExecutionTimeHeader( program_execution_time );
}

// Print the unit test stats
void UnitTestManager::printUnitTestStats( const std::string& summary_header )
{
  this->printGivenUnitTestStats( summary_header,
                                 d_data->getUnitTests().size(),
                                 d_data->getRunTestCounter(),
                                 d_data->getPassedTestCounter(),
                                 d_data->getTotalUnitTestExecutionTime );
}

// Print the unit test stats
void DistributedUnitTestManager::printUnitTestStats(
                                            const std::string& summary_header )
{
  if( Utility::GlobalMPISession::getSize() == 1 )
    UnitTestManager::printUnitTestStats( summary_header );
  else
  {
    for( int i = 0; i < Utility::GlobalMPISession::getSize(); ++i )
    {
      if( Utility::GlobalMPISession::getRank() == i )
      {
        std::ostringstream oss;
        oss << summary_header << " For Proc " << i;
        
        UnitTestManager::printUnitTestStats( oss.str() );
      }

      Utility::GlobalMPISession::barrier();
    }

    // Reduce the test stats
    size_t reduced_total = mpi_session.sum(this->getNumberOfTests());
    size_t reduced_run_counter = mpi_session.sum(this->getNumberOfRunTests());
    size_t reduced_passed_counter =
      mpi_session.sum(this->getNumberOfPassedTests());

    if( Utility::GlobalMPISession::getRank() == 0 )
    {
      this->printGivenUnitTestStats( summary_header + " For All Procs",
                                     reduced_total,
                                     reduced_run_counter,
                                     reduced_passed_counter,
                                     -1.0 );
    }

    Utility::GlobalMPISession::barrier();
  }
}

// Print the unit test stats
void UnitTestManager::printGivenUnitTestStats(
                                           const std::string& summary_header,
                                           const size_t number_of_tests,
                                           const size_t number_of_tests_run,
                                           const size_t number_of_tests_passed,
                                           const double total_test_exec_time )
{
  d_data->getReportSink() << Utility::Bold(summary_header + ":") << " "
                          << "total: " << number_of_tests << " "
                          << Utility::Magenta("skipped:") << " "
                          << number_of_tests - number_of_tests_run
                          << "run: " << number_of_tests_run << " "
                          << Utility::Green("passed:") << " "
                          << number_of_tests_passed << " "
                          << Utility::Red("failed:") << " "
                          << number_of_tests_run - number_of_tests_passed;

  if( total_test_exec_time >= 0.0 )
    d_data->getReportSink() << " time: " << total_test_exec_time;

  d_data->getReportSink() << std::endl;
}

// Print the program execution time header
void UnitTestManager::printProgramExecutionTimeHeader(
                                          const double program_execution_time )
{
  d_data->getReportSink() << "\nProgram Execution Time: ";

  UnitTestManager::printOperationTime( program_execution_time, false, true );
}

// Print the program execution time header
void DistributedUnitTestManager::printProgramExecutionTimeHeader(
                                          const double program_execution_time )
{
  if( Utility::GlobalMPISession::getRank() == 0 )
    UnitTestManager::printProgramExecutionTimeHeader( program_execution_time );

  Utility::GlobalMPISession::barrier();
}

// Return the number of tests
size_t UnitTestManager::getNumberOfTests() const
{
  return d_data->getUnitTests().size();
}

// Return the current number of run tests
size_t UnitTestManager::getNumberOfRunTests() const
{
  return d_data->getRunTestCounter();
}

// Return the current number of passed tests
size_t UnitTestManager::getNumberOfPassedTests() const
{
  return d_data->getPassedTestCounter();
}

// Summarize the results
int UnitTestManager::summarizeResults( const bool local_success,
                                       const bool global_success ) const
{
  this->printTestResult( "End Result", local_success )

  return (global_success ? 0 : 1);
}

// Print the test result header
void UnitTestManager::printTestResult( const std::string& header,
                                       const bool success,
                                       const bool )
{
  d_data->getReportSink() << "\n" << header << ": ";

  if( success )
    d_data->getReportSink() << Utility::Green("TEST PASSED");
  else
    d_data->getReportSink() << Utility::Red("TEST FAILED");

  d_data->getReportSink() << std::endl;
}

// Print the test result header
void DistributedUnitTestManager::printTestResult( const std::string& header,
                                                  const bool local_success,
                                                  const bool global_success )
{
  if( Utility::GlobalMPISession::getSize() == 1 )
    UnitTestManager::printTestResult( header, local_success, global_success );
  else
  {
    // Print the result for each process
    for( int i = 0; i < Utility::GlobalMPISession::getSize(); ++i )
    {
      if( Utility::GlobalMPISession::getRank() == i )
      {
        std::ostringstream oss;
        oss << header << " For Proc " << i;

        UnitTestManager::printTestResult( oss.str(),
                                          local_success,
                                          global_success );
      }

      Utility::GlobalMPISession::barrier();
    }

    // Print the overall result
    UnitTestManager::PrintTestResult( "\n"+header, global_success, global_success );
  }
}

// Constructor
UnitTestManager::Initializer::Initializer()
  : setOption( UnitTestManager::getInstance().d_data->d_custom_command_line_options )
{ /* ... */ }

// Initialize the unit test manager
void UnitTestManager::Initializer::initializeUnitTestManager( int argc, char** argv )
{
  // Set the custom command line options
  this->setCustomCommandLineOptions();

  // Parse the command line options
  UnitTestManager::getInstance().d_data->parseCommandLineOptions( argc, argv );

  // Initialize the manager
  this->customUnitTestManagerInitialization();
}

// Get command line option value
boost::program_options::variable_value
getOptionValue( const std::string& option_name ) const
{
  return UnitTestManager::getInstance().d_data->getCommandLineOptionValue( option_name );
}    

// Set the custom command line options
/*! \details Derived classes can override this method if a unit test suite
 * needs additional command line options. Note: this method is non-const
 * because of how it can be used by derived classes.
 */
void UnitTestManager::Initializer::setCustomCommandLineOptions()
{ /* ... */ }

// Custom manager initialization method
/*! \details Derived classes can override this method if test data needs
 * to be initialized in a specific way.
 */
void UnitTestManager::Initializer::customUnitTestManagerInitialization() const
{ /* ... */ }
  
} // end Utility namespace

//---------------------------------------------------------------------------//
// end Utility_UnitTestManager.cpp
//---------------------------------------------------------------------------//
