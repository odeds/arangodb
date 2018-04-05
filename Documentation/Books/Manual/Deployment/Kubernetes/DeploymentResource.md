<!-- don't edit here, its from https://@github.com/arangodb/kube-arangodb.git / docs/Manual/ -->
# ArangoLocalStorage Custom Resource

The ArangoDB Deployment Operator creates and maintains ArangoDB deployments
in a Kubernetes cluster, given a deployment specification.
This deployment specification is a `CustomResource` following
a `CustomResourceDefinition` created by the operator.

Example minimal deployment definition of an ArangoDB database cluster:

```yaml
apiVersion: "database.arangodb.com/v1alpha"
kind: "ArangoDeployment"
metadata:
  name: "example-arangodb-cluster"
spec:
  mode: Cluster
```

Example more elaborate deployment definition:

```yaml
apiVersion: "database.arangodb.com/v1alpha"
kind: "ArangoDeployment"
metadata:
  name: "example-arangodb-cluster"
spec:
  mode: Cluster
  environment: Production
  agents:
    count: 3
    args:
      - --log.level=debug
    resources:
      requests:
        storage: 8Gi
    storageClassName: ssd
  dbservers:
    count: 5
    resources:
      requests:
        storage: 80Gi
    storageClassName: ssd
  coordinators:
    count: 3
  image: "arangodb/arangodb:3.3.4"
```

## Specification reference

Below you'll find all settings of the `ArangoDeployment` custom resource.
Several settings are for various groups of servers. These are indicated
with `<group>` where `<group>` can be any of:

- `agents` for all agents of a `Cluster` or `ResilientSingle` pair.
- `dbservers` for all dbservers of a `Cluster`.
- `coordinators` for all coordinators of a `Cluster`.
- `single` for all single servers of a `Single` instance or `ResilientSingle` pair.
- `syncmasters` for all syncmasters of a `Cluster`.
- `syncworkers` for all syncworkers of a `Cluster`.

### `spec.mode: string`

This setting specifies the type of deployment you want to create.
Possible values are:

- `Cluster` (default) Full cluster. Defaults to 3 agents, 3 dbservers & 3 coordinators.
- `ResilientSingle` Resilient single pair. Defaults to 3 agents and 2 single servers.
- `Single` Single server only (note this does not provide high availability or reliability).

This setting cannot be changed after the deployment has been created.

### `spec.environment: string`

This setting specifies the type of environment in which the deployment is created.
Possible values are:

- `Development` (default) This value optimizes the deployment for development
  use. It is possible to run a deployment on a small number of nodes (e.g. minikube).
- `Production` This value optimizes the deployment for production use.
  It puts required affinity constraints on all pods to avoid agents & dbservers
  from running on the same machine.

### `spec.image: string`

This setting specifies the docker image to use for all ArangoDB servers.
In a `development` environment this setting defaults to `arangodb/arangodb:latest`.
For `production` environments this is a required setting without a default value.
It is highly recommend to use explicit version (not `latest`) for production
environments.

### `spec.imagePullPolicy: string`

This setting specifies the pull policy for the docker image to use for all ArangoDB servers.
Possible values are:

- `IfNotPresent` (default) to pull only when the image is not found on the node.
- `Always` to always pull the image before using it.

### `spec.storageEngine: string`

This setting specifies the type of storage engine used for all servers
in the cluster.
Possible values are:

- `MMFiles` To use the MMFiles storage engine.
- `RocksDB` (default) To use the RocksDB storage engine.

This setting cannot be changed after the cluster has been created.

### `spec.rocksdb.encryption.keySecretName`

This setting specifies the name of a kubernetes `Secret` that contains
an encryption key used for encrypting all data stored by ArangoDB servers.
When an encryption key is used, encryption of the data in the cluster is enabled,
without it encryption is disabled.
The default value is empty.

This requires the Enterprise version.

The encryption key cannot be changed after the cluster has been created.

The secret specified by this setting, must have a data field named 'key' containing
an encryption key that is exactly 32 bytes long.

### `spec.auth.jwtSecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
the JWT token used for accessing all ArangoDB servers.
When no name is specified, it defaults to `<deployment-name>-jwt`.
To disable authentication, set this value to `None`.

If you specify a name of a `Secret`, that secret must have the token
in a data field named `token`.

If you specify a name of a `Secret` that does not exist, a random token is created
and stored in a `Secret` with given name.

Changing a JWT token results in stopping the entire cluster
and restarting it.

### `spec.tls.caSecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
a standard CA certificate + private key used to sign certificates for individual
ArangoDB servers.
When no name is specified, it defaults to `<deployment-name>-ca`.
To disable authentication, set this value to `None`.

If you specify a name of a `Secret` that does not exist, a self-signed CA certificate + key is created
and stored in a `Secret` with given name.

The specified `Secret`, must contain the following data fields:

- `ca.crt` PEM encoded public key of the CA certificate
- `ca.key` PEM encoded private key of the CA certificate

### `spec.tls.altNames: []string`

This setting specifies a list of alternate names that will be added to all generated
certificates. These names can be DNS names or email addresses.
The default value is empty.

### `spec.tls.ttl: duration`

This setting specifies the time to live of all generated
server certificates.
The default value is `2160h` (about 3 month).

When the server certificate is about to expire, it will be automatically replaced
by a new one and the affected server will be restarted.

Note: The time to live of the CA certificate (when created automatically)
will be set to 10 years.

### `spec.sync.enabled: bool`

This setting enables/disables support for data center 2 data center
replication in the cluster. When enabled, the cluster will contain
a number of `syncmaster` & `syncworker` servers.
The default value is `false`.

### `spec.sync.image: string`

This setting specifies the docker image to use for all ArangoSync servers.
When not specified, the `spec.image` value is used.

### `spec.sync.imagePullPolicy: string`

This setting specifies the pull policy for the docker image to use for all ArangoSync servers.
For possible values, see `spec.imagePullPolicy`.
When not specified, the `spec.imagePullPolicy` value is used.

### `spec.sync.auth.jwtSecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
the JWT token used for accessing all ArangoSync master servers.
When not specified, the `spec.auth.jwtSecretName` value is used.

If you specify a name of a `Secret` that does not exist, a random token is created
and stored in a `Secret` with given name.

### `spec.sync.auth.clientCASecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
a PEM encoded CA certificate used for client certificate verification
in all ArangoSync master servers.
This is a required setting when `spec.sync.enabled` is `true`.
The default value is empty.

### `spec.sync.mq.type: string`

This setting sets the type of message queue used by ArangoSync.
Possible values are:

- `Direct` (default) for direct HTTP connections between the 2 data centers.

### `spec.sync.tls.caSecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
a standard CA certificate + private key used to sign certificates for individual
ArangoSync master servers.

When no name is specified, it defaults to `<deployment-name>-sync-ca`.

If you specify a name of a `Secret` that does not exist, a self-signed CA certificate + key is created
and stored in a `Secret` with given name.

The specified `Secret`, must contain the following data fields:

- `ca.crt` PEM encoded public key of the CA certificate
- `ca.key` PEM encoded private key of the CA certificate

### `spec.sync.tls.altNames: []string`

This setting specifies a list of alternate names that will be added to all generated
certificates. These names can be DNS names or email addresses.
The default value is empty.

### `spec.sync.monitoring.tokenSecretName: string`

This setting specifies the name of a kubernetes `Secret` that contains
the bearer token used for accessing all monitoring endpoints of all ArangoSync
servers.
When not specified, no monitoring token is used.
The default value is empty.

### `spec.ipv6.forbidden: bool`

This setting prevents the use of IPv6 addresses by ArangoDB servers.
The default is `false`.

### `spec.<group>.count: number`

This setting specifies the number of servers to start for the given group.
For the agent group, this value must be a positive, odd number.
The default value is `3` for all groups except `single` (there the default is `1`
for `spec.mode: single` and `2` for `spec.mode: resilientsingle`).

For the `syncworkers` group, it is highly recommended to use the same number
as for the `dbservers` group.

### `spec.<group>.args: [string]`

This setting specifies additional commandline arguments passed to all servers of this group.
The default value is an empty array.

### `spec.<group>.resources.requests.cpu: cpuUnit`

This setting specifies the amount of CPU requested by server of this group.

See https://kubernetes.io/docs/concepts/configuration/manage-compute-resources-container/ for details.

### `spec.<group>.resources.requests.memory: memoryUnit`

This setting specifies the amount of memory requested by server of this group.

See https://kubernetes.io/docs/concepts/configuration/manage-compute-resources-container/ for details.

### `spec.<group>.resources.requests.storage: storageUnit`

This setting specifies the amount of storage required for each server of this group.
The default value is `8Gi`.

This setting is not available for group `coordinators`, `syncmasters` & `syncworkers`
because servers in these groups do not need persistent storage.

### `spec.<group>.storageClassName: string`

This setting specifies the `storageClass` for the `PersistentVolume`s created
for each server of this group.

This setting is not available for group `coordinators`, `syncmasters` & `syncworkers`
because servers in these groups do not need persistent storage.