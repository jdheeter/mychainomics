const eosjs = require('../eosjs')
const env = require('../.env')
const accts = require('../accounts')
const acct = (name) => accts[env.network][name]
const api = eosjs(env.keys[env.network]).api
const tapos = { blocksBehind: 6, expireSeconds: 10 }

async function doAction(name,data,auth){
  try {
    if (!data) data = {}
    const contract = auth
    const authorization = [{actor:auth,permission: 'active'}]
    const account = contract
    const result = await api.transact({actions: [{ account, name, data, authorization }]},tapos)
    console.log(result.transaction_id)
    return result
  } catch (error) {
    console.error(error.toString())
  }
}

async function adduser(owner,chainomics_id){
  return doAction('adduser', { owner, chainomics_id }, acct('accounts')).then(result => console.log(result))
}

async function addconsumer (owner, consumer_id) {
  return doAction('addconsumer', { owner, consumer_id }, acct('accounts')).then(result => console.log(result))
}

async function setusermeta (chainomics_id, new_user_meta) {
  return doAction('setusermeta', { chainomics_id, new_user_meta }, acct('accounts')).then(result => console.log(result))
}
async function providermeta (provider_id, new_provider_meta) {
  return doAction('providermeta', { provider_id, new_provider_meta }, acct('accounts')).then(result => console.log(result))
}
async function consumermeta (consumer_id, new_consumer_meta) {
  return doAction('consumermeta', { consumer_id, new_consumer_meta }, acct('accounts')).then(result => console.log(result))
}

async function addprovider (owner, provider_id) {
  return doAction('addprovider', { owner, provider_id }, acct('accounts')).then(result => console.log(result))
}

async function authdataprov (chainomics_id, provider_id, remove) {
  return doAction('authdataprov', { chainomics_id, provider_id, remove: JSON.parse(remove) }, acct('accounts')).then(result => console.log(result))
}

async function authdatacons (chainomics_id, consumer_id, remove) {
  return doAction('authdatacons', { chainomics_id, consumer_id, remove: JSON.parse(remove) }, acct('accounts')).then(result => console.log(result))
}

async function verifyuser (chainomics_id, id_verification_hash) {
  return doAction('verifyuser', { chainomics_id, id_verification_hash }, acct('accounts')).then(result => console.log(result))
}

async function freezeuser (chainomics_id, freeze) {
  return doAction('freezeuser', { chainomics_id, freeze }, acct('accounts')).then(result => console.log(result))
}
async function reset(){
  doAction('delusers', null, acct('accounts')).then(result => console.log(result))
  doAction('deltokens',null,acct('accounts')).then(result => console.log(result))
  doAction('delproviders',null,acct('accounts')).then(result => console.log(result))
  doAction('delconsumers',null,acct('accounts')).then(result => console.log(result))

}
async function addtoken(contract,quantity){
  return doAction('addtoken',{extended_asset:{contract,quantity}},acct('accounts')).then(result => console.log(result))
}

async function internalxfer(from_chainomic_account,to_chainomic_account,quantity,memo){
  return doAction('internalxfer',{from_chainomic_account,to_chainomic_account,quantity,memo},acct('accounts')).then(result => console.log(result))
}

async function externalxfer(from_chainomic_account,to_chain_account,quantity,memo){
  return doAction('externalxfer',{from_chainomic_account,to_chain_account,quantity,memo},acct('accounts')).then(result => console.log(result))
}

async function setlevel (chainomic_account, level) {
  return doAction('setlevel', { chainomic_account, level }, acct('accounts')).then(result => console.log(result))
}

const methods = { adduser, reset, addtoken, internalxfer, externalxfer, authdataprov, 
  verifyuser, setlevel, freezeuser, authdatacons, addconsumer,
  addprovider, setusermeta, providermeta, consumermeta }

if(process.argv[2] && require.main === module) {
  if (Object.keys(methods).find(el => el === process.argv[2])) {
    console.log("Starting:",process.argv[2])
    methods[process.argv[2]](process.argv[3],process.argv[4],process.argv[5],process.argv[6]).catch(console.error)
    .then((result)=>console.log('Finished'))
  }
}

module.exports = methods
