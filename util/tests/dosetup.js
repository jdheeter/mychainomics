const contract = require('../do')


async function init(){
  try {
    await contract.adduser("mychainomics","testusr2")
    await contract.adduser("mychainomics","testusr3")
    await contract.adduser("mychainomics","testusr4")

  } catch (error) {
    console.error(error.toString())
  }
}

init().catch(console.log)
