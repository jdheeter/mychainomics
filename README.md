
## Build Instructions

export CONTRACTNAME=mychainomics

eosio-cpp $CONTRACTNAME.cpp -o ../$CONTRACTNAME.wasm -abigen -I ../include -I ./ -O 2 -lto-opt O2

After the contract is built and deployed, the contract is ready for data registration.  \
 \
The contracts can be interacted with directly or using the convenient nodejs wrapper functions (in the utils folder). The “do.js” script can be used from the command like “node do adduser mychainomics userid invitedid’. To use the nodejs scripts you need to create .env.js in the project root directory (look at example.env.js for reference). \
 \
To try the contract the typical order of operations would be \
**User Setup**

adduser \
Example: https://kylin.bloks.io/transaction/3d70dc5f0a10312602ad87f7a85a667a450b4d3a06c8ee114f91409e244acb72\
setusermeta \
Example: https://kylin.bloks.io/transaction/86fa65a05cb36da94578bf2d190fef8a718a8d04abc706e128f7067b0153a17d
verifyuser \
Example: https://kylin.eosq.app/tx/b3f993e14284cafbcc7327cede76a07b404d00b2e95007e12c5816c97789afe5 \
 \
**Token setup** (optional) \
addtoken 
Example: https://kylin.eosq.app/tx/789f7f151f82aeca93988afc4c9e76198ebe7e58dd89e0f32cda11e54727502f


**Data consumer/provider setup** \
addconsumer \
Example: https://kylin.eosq.app/tx/cef429786e8c1703be3452badcafae5e249db6f202e5ac98a08d0a5c416f3c75\

addprovider

 \
**Register events (Clinical trials)** \
Createevent (using a data consumer account as the owner) \
 Example: https://kylin.eosq.app/tx/4fff60b1197a44f200257bf7f163506c2acd089991ed0952ee2019746228d9b0\
**Users apply to participate in event** \
applytoevent (using a user account) \
Example:https://kylin.eosq.app/tx/97e33f2fd3eb2ef281cfbdda2358dbc38654386129a4e90a595349e413bf23e1\

**Reward participants after an event has finished** \
setlevel \
Example: https://kylin.eosq.app/tx/f09d91f9091680011c0a5c926be8924ca56042c1ad2d8e7d37b2c799265a2078\
internalxfer \
 Example:\
verifypartic\
Example: https://kylin.eosq.app/tx/6e4e67a206e8ab4034e3200766933046e773131d506741d1156d40ff4a97530a\
Breakdown of primary action functionality: \
 adduser

Name: 
**owner**


        The chain account responsible for signing transactions for this chainomics account (the internal subaccount) Can be any chain account, one chain account could be the owner of many subaccounts.

Name:
**chainomics_id**(Index)


        The unique identifier for this user account. This is how the user is identified inside the contract when interacting with other aspects of the system.


    Name:\
    **invited_by**


        A unique id meant to represent the user (chainomics_id) who invited this user. The referral system can use this information to reward participants.

  addtoken


    extended_asset: **extended_asset**


        Register an extended_asset (the token symbol and contract name) to be used inside the system. Tokens which are not whitelisted using this action won’t be usable in mychainomics.

 Internalxfer

For transferring coins in between internal subaccounts 

 externalxfer


    For transferring coins out of the contract from a subaccount (initiates an inline transfer action to an external eosio account)

 setlevel


    The system can set the level of a subaccount, used for the internal gamification system.

  verifyuser


    After a user has registered and submitted identification information the user can be marked as “verified”. The action includes a string which is a hash of the users identification details.

freezeuser

	Give the admin the ability to freeze accounts under special circumstances.

setusermeta

Update the user metadata, should be a JSON string.

authdataprov


    Users can whitelist a data provider who then has permission to share data with data consumers.

authdatacons

	Users can whitelist a data consumer who then has permission access data from data providers.

addconsumer

Register a data consumer.

 addprovider

	Register a data provider.

 providermeta

	Update provider metadata.

  consumermeta

	Update consumer metadata.

 createevent


    A data consumer can register events for users to apply for

 applytoevent


    Users can apply to participate in an event. \


**Administrative actions for debugging/testing**

  delusers  deltokens  delconsumers  delproviders \
 \
 \
