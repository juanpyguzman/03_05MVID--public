#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class Wallet {
  public:
    uint32_t getMoney() const {
      return _money;
    }

    void addMoney(const uint32_t amount) {
      std::lock_guard<std::mutex> guard(_mutex);
      for (uint32_t i = 0; i < amount; ++i) {
        ++_money;
      }
    }

  private:
    uint32_t _money = 0;
    std::mutex _mutex;
};


/* t0
 * money = 20000
 *
 * thread 1 core 1                          thread 2 core 2
 *
 * load money -> register 20000
 *                                          load money -> register 20000
 *
 * money = 20000
 * increment register 20001
 *                                          increment register 20001
 * money = 20000
 *                                          update money <- register 20001
 * money = 20001
 * update money <- register 20001
 *
 * money = 20001
 *
 *
 *
 *
 * tn
 */



uint32_t testWallet() {
  Wallet wallet;
  std::vector<std::thread> threads;
  for (uint32_t i = 0; i < 20; ++i) {
    threads.push_back(std::thread(&Wallet::addMoney, &wallet, 10000));
  }

  for (auto& t : threads) {
    t.join();
  }

  return wallet.getMoney();
}


int main() {
  std::cout << " Start" << std::endl;
  for(uint32_t i = 0; i < 100; ++i) {
    const uint32_t val = testWallet();
    if (val != 200000) {
      std::cout << "Error in simulation " << i << ", Money in Wallet is " << val << std::endl;
    }
  }

  std::cout << " End" << std::endl;
  return 0;
}