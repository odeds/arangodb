////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Jan Steemann
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_CLUSTER_SERVER_STATE_H
#define ARANGOD_CLUSTER_SERVER_STATE_H 1

#include "Basics/Common.h"
#include "Basics/ReadWriteLock.h"

namespace arangodb {

class ServerState {
  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief an enum describing the roles a server can have
  //////////////////////////////////////////////////////////////////////////////

  enum RoleEnum : int {
    ROLE_UNDEFINED = 0,  // initial value
    ROLE_SINGLE,         // is set when cluster feature is off
    ROLE_PRIMARY,
    ROLE_SECONDARY,
    ROLE_COORDINATOR
  };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief an enum describing the possible states a server can have
  //////////////////////////////////////////////////////////////////////////////

  enum StateEnum {
    STATE_UNDEFINED = 0,  // initial value
    STATE_STARTUP,        // used by all roles
    STATE_SERVINGASYNC,   // primary only
    STATE_SERVINGSYNC,    // primary only
    STATE_STOPPING,       // primary only
    STATE_STOPPED,        // primary only
    STATE_SYNCING,        // secondary only
    STATE_INSYNC,         // secondary only
    STATE_LOSTPRIMARY,    // secondary only
    STATE_SERVING,        // coordinator only
    STATE_SHUTDOWN        // used by all roles
  };

  
 public:

  ServerState();


  ~ServerState();

  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief create the (sole) instance
  //////////////////////////////////////////////////////////////////////////////

  static ServerState* instance();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the string representation of a role
  //////////////////////////////////////////////////////////////////////////////

  static std::string roleToString(RoleEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief convert a string to a role
  //////////////////////////////////////////////////////////////////////////////

  static RoleEnum stringToRole(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the string representation of a state
  //////////////////////////////////////////////////////////////////////////////

  static std::string stateToString(StateEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief convert a string representation to a state
  //////////////////////////////////////////////////////////////////////////////

  static StateEnum stringToState(std::string const&);

  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the initialized flag
  //////////////////////////////////////////////////////////////////////////////

  void setInitialized() { _initialized = true; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief whether or not the cluster was properly initialized
  //////////////////////////////////////////////////////////////////////////////

  bool initialized() const { return _initialized; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the initialized flag
  //////////////////////////////////////////////////////////////////////////////

  void setClusterEnabled() { _clusterEnabled = true; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the authentication data for cluster-internal communication
  //////////////////////////////////////////////////////////////////////////////

  void setAuthentication(std::string const&, std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the authentication data for cluster-internal communication
  //////////////////////////////////////////////////////////////////////////////

  std::string getAuthentication();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief flush the server state (used for testing)
  //////////////////////////////////////////////////////////////////////////////

  void flush();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief check whether the server is a coordinator
  //////////////////////////////////////////////////////////////////////////////

  bool isCoordinator();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief check whether the server is a coordinator
  //////////////////////////////////////////////////////////////////////////////

  static bool isCoordinator(RoleEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief check whether the server is a DB server (primary or secondary)
  /// running in cluster mode.
  //////////////////////////////////////////////////////////////////////////////

  bool isDBServer();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief check whether the server is a DB server (primary or secondary)
  /// running in cluster mode.
  //////////////////////////////////////////////////////////////////////////////

  static bool isDBServer(RoleEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief check whether the server is running in a cluster
  //////////////////////////////////////////////////////////////////////////////

  bool isRunningInCluster();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the server role
  //////////////////////////////////////////////////////////////////////////////

  RoleEnum getRole();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the server role
  //////////////////////////////////////////////////////////////////////////////

  void setRole(RoleEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the server local info
  //////////////////////////////////////////////////////////////////////////////

  std::string getLocalInfo();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the server id
  //////////////////////////////////////////////////////////////////////////////

  std::string getId();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief for a secondary get the server id of its primary
  //////////////////////////////////////////////////////////////////////////////

  std::string getPrimaryId();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the server description
  //////////////////////////////////////////////////////////////////////////////

  std::string getDescription();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the server local info
  //////////////////////////////////////////////////////////////////////////////

  void setLocalInfo(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the server id
  //////////////////////////////////////////////////////////////////////////////

  void setId(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the server description
  //////////////////////////////////////////////////////////////////////////////

  void setDescription(std::string const& description);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the server address
  //////////////////////////////////////////////////////////////////////////////

  std::string getAddress();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the server address
  //////////////////////////////////////////////////////////////////////////////

  void setAddress(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the current state
  //////////////////////////////////////////////////////////////////////////////

  StateEnum getState();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set the current state
  //////////////////////////////////////////////////////////////////////////////

  void setState(StateEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the data path
  //////////////////////////////////////////////////////////////////////////////

  std::string getDataPath();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the data path
  //////////////////////////////////////////////////////////////////////////////

  void setDataPath(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the log path
  //////////////////////////////////////////////////////////////////////////////

  std::string getLogPath();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the log path
  //////////////////////////////////////////////////////////////////////////////

  void setLogPath(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the agent path
  //////////////////////////////////////////////////////////////////////////////

  std::string getAgentPath();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the data path
  //////////////////////////////////////////////////////////////////////////////

  void setAgentPath(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the arangod path
  //////////////////////////////////////////////////////////////////////////////

  std::string getArangodPath();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the arangod path
  //////////////////////////////////////////////////////////////////////////////

  void setArangodPath(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the DBserver config
  //////////////////////////////////////////////////////////////////////////////

  std::string getDBserverConfig();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the DBserver config
  //////////////////////////////////////////////////////////////////////////////

  void setDBserverConfig(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the coordinator config
  //////////////////////////////////////////////////////////////////////////////

  std::string getCoordinatorConfig();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the coordinator config
  //////////////////////////////////////////////////////////////////////////////

  void setCoordinatorConfig(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the JavaScript startup path
  //////////////////////////////////////////////////////////////////////////////

  std::string getJavaScriptPath();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the JavaScript startup path
  //////////////////////////////////////////////////////////////////////////////

  void setJavaScriptPath(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the disable dispatcher frontend flag
  //////////////////////////////////////////////////////////////////////////////

  bool getDisableDispatcherFrontend();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the disable dispatcher frontend flag
  //////////////////////////////////////////////////////////////////////////////

  void setDisableDispatcherFrontend(bool);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief gets the disable dispatcher kickstarter flag
  //////////////////////////////////////////////////////////////////////////////

  bool getDisableDispatcherKickstarter();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the disable dispatcher kickstarter flag
  //////////////////////////////////////////////////////////////////////////////

  void setDisableDispatcherKickstarter(bool);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief redetermine the server role, we do this after a plan change.
  /// This is needed for automatic failover. This calls determineRole with
  /// previous values of _info and _id. In particular, the _id will usually
  /// already be set. If the current role cannot be determined from the
  /// agency or is not unique, then the system keeps the old role.
  /// Returns true if there is a change and false otherwise.
  //////////////////////////////////////////////////////////////////////////////

  bool redetermineRole();

  
 private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief atomically fetches the server role
  //////////////////////////////////////////////////////////////////////////////

  RoleEnum loadRole() {
    return static_cast<RoleEnum>(_role.load(std::memory_order_consume));
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief atomically stores the server role
  //////////////////////////////////////////////////////////////////////////////

  void storeRole(RoleEnum role) {
    _role.store(role, std::memory_order_release);
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief determine the server role
  //////////////////////////////////////////////////////////////////////////////

  RoleEnum determineRole(std::string const& info, std::string& id);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief lookup the server id by using the local info
  //////////////////////////////////////////////////////////////////////////////

  int lookupLocalInfoToId(std::string const& localInfo, std::string& id);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief lookup the server role by scanning Plan/Coordinators for our id
  //////////////////////////////////////////////////////////////////////////////

  ServerState::RoleEnum checkCoordinatorsList(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief lookup the server role by scanning Plan/DBServers for our id
  //////////////////////////////////////////////////////////////////////////////

  ServerState::RoleEnum checkServersList(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief validate a state transition for a primary server
  //////////////////////////////////////////////////////////////////////////////

  bool checkPrimaryState(StateEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief validate a state transition for a secondary server
  //////////////////////////////////////////////////////////////////////////////

  bool checkSecondaryState(StateEnum);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief validate a state transition for a coordinator server
  //////////////////////////////////////////////////////////////////////////////

  bool checkCoordinatorState(StateEnum);

  
 private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief the pointer to the singleton instance
  //////////////////////////////////////////////////////////////////////////////

  static ServerState* _theinstance;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the server's local info, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _localInfo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the server's id, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _id;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the server's description
  //////////////////////////////////////////////////////////////////////////////

  std::string _description;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the data path, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _dataPath;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the log path, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _logPath;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the agent path, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _agentPath;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the arangod path, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _arangodPath;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the JavaScript startup path, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _javaScriptStartupPath;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the DBserver config, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _dbserverConfig;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the coordinator config, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _coordinatorConfig;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the disable dispatcher frontend flag, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  bool _disableDispatcherFrontend;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the disable dispatcher kickstarter flag, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  bool _disableDispatcherKickstarter;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the server's own address, can be set just once
  //////////////////////////////////////////////////////////////////////////////

  std::string _address;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the authentication data used for cluster-internal communication
  //////////////////////////////////////////////////////////////////////////////

  std::string _authentication;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief r/w lock for state
  //////////////////////////////////////////////////////////////////////////////

  arangodb::basics::ReadWriteLock _lock;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the server role
  //////////////////////////////////////////////////////////////////////////////

  std::atomic<int> _role;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief a secondary stores the ID of its primary here:
  //////////////////////////////////////////////////////////////////////////////

  std::string _idOfPrimary;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the current state
  //////////////////////////////////////////////////////////////////////////////

  StateEnum _state;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief whether or not the cluster was initialized
  //////////////////////////////////////////////////////////////////////////////

  bool _initialized;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief whether or not we are a cluster member
  //////////////////////////////////////////////////////////////////////////////

  bool _clusterEnabled;
};
}

#endif


