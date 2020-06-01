const contract = require('../do')

const usermetadata = require('./usermeta.json')
const consumermeta = require('./consumermeta.json')
const eventsmeta = require('./eventmeta.json')

function makeid(length) {
  var result           = '';
  var characters       = 'abcdefghijklmnopqrstuvwxyz12345';
  var charactersLength = characters.length;
  for ( var i = 0; i < length; i++ ) {
     result += characters.charAt(Math.floor(Math.random() * charactersLength));
  }
  return result;
}


async function addDummyUsers(){
  try {
    for (user of usermetadata) {
      const id = makeid(12)
      await contract.adduser('mychainomics',id,makeid(12))
      await contract.setusermeta(id,JSON.stringify(user))
    }f
  } catch (error) {
    console.error(error.toString())
  }
}

// async function addDummyProviders(){
//   try {
//     for (user of usermetadata) {
//       const id = makeid(12)
//       await contract.adduser('mychainomics',id,makeid(12))
//       await contract.setusermeta(id,user)
//     }f
//   } catch (error) {
//     console.error(error.toString())
//   }
// }


// async function addDummyConsumers(){
//   try {
//     for (consumer of consumermeta) {
//       const id = makeid(12)
//       await contract.addconsumer('mychainomics',id)
//       await contract.consumermeta(id,consumer)
//     }
//   } catch (error) {
//     console.error(error.toString())
//   }
// } 


async function addDummyEvents(){
  try {
    for (event of eventsmeta) {
      const id = makeid(12)
      await contract.createevent(event.owner,id,event.start,event.end,event)
    }
  } catch (error) {
    console.error(error.toString())
  }
}


// addDummyUsers().catch(console.log)
// addDummyConsumers().catch(console.log)
addDummyEvents().catch(console.log)

