# LTCVanitySearch

LTCVanitySearch is a Litecoin address prefix finder, forked from [VanitySearch](https://github.com/JeanLucPons/VanitySearch) by Jean Luc PONS. If you want to generate safe private keys, use the -s option to enter your passphrase which will be used for generating a base key as for BIP38 standard (*LTCVanitySearch.exe -s "My PassPhrase" LMyPrefix*). You can also use *LTCVanitySearch.exe -ps "My PassPhrase"* which will add a crypto secure seed to your passphrase.\
LTCVanitySearch may not compute a good grid size for your GPU, so try different values using -g option in order to get the best performances. If you want to use GPUs and CPUs together, you may have best performances by keeping one CPU core for handling GPU(s)/CPU exchanges (use -t option to set the number of CPU threads).

## Changes from VanitySearch

- All address generation uses Litecoin parameters (P2PKH version `0x30`, P2SH version `0x32`, bech32 HRP `ltc`, WIF prefix `0xB0`)
- Litecoin P2PKH addresses start with `L`, P2SH with `M`, bech32 with `ltc1q`
- Updated to CUDA 12.8 with native support for RTX 5090 (SM 120 / Blackwell)
- Supports Volta, Turing, Ampere, Ada Lovelace, and Blackwell GPU architectures

# Feature

<ul>
  <li>Fixed size arithmetic</li>
  <li>Fast Modular Inversion (Delayed Right Shift 62 bits)</li>
  <li>SecpK1 Fast modular multiplication (2 steps folding 512bits to 256bits using 64 bits digits)</li>
  <li>Use some properties of elliptic curve to generate more keys</li>
  <li>SSE Secure Hash Algorithm SHA256 and RIPEMD160 (CPU)</li>
  <li>Multi-GPU support</li>
  <li>CUDA optimisation via inline PTX assembly</li>
  <li>Seed protected by pbkdf2_hmac_sha512 (BIP38)</li>
  <li>Support P2PKH, P2SH and BECH32 Litecoin addresses</li>
  <li>Support split-key vanity address</li>
</ul>

# Usage

```
LTCVanitySearch [-check] [-v] [-u] [-b] [-c] [-gpu] [-stop] [-i inputfile]
             [-gpuId gpuId1[,gpuId2,...]] [-g g1x,g1y,[,g2x,g2y,...]]
             [-o outputfile] [-m maxFound] [-ps seed] [-s seed] [-t nbThread]
             [-nosse] [-r rekey] [-check] [-kp] [-sp startPubKey]
             [-rp privkey partialkeyfile] [prefix]

 prefix: prefix to search (Can contains wildcard '?' or '*')
 -v: Print version
 -u: Search uncompressed addresses
 -b: Search both uncompressed or compressed addresses
 -c: Case unsensitive search
 -gpu: Enable gpu calculation
 -stop: Stop when all prefixes are found
 -i inputfile: Get list of prefixes to search from specified file
 -o outputfile: Output results to the specified file
 -gpu gpuId1,gpuId2,...: List of GPU(s) to use, default is 0
 -g g1x,g1y,g2x,g2y, ...: Specify GPU(s) kernel gridsize, default is 8*(MP number),128
 -m: Specify maximun number of prefixes found by each kernel call
 -s seed: Specify a seed for the base key, default is random
 -ps seed: Specify a seed concatened with a crypto secure random seed
 -t threadNumber: Specify number of CPU thread, default is number of core
 -nosse: Disable SSE hash function
 -l: List cuda enabled devices
 -check: Check CPU and GPU kernel vs CPU
 -cp privKey: Compute public key (privKey in hex hormat)
 -kp: Generate key pair
 -rp privkey partialkeyfile: Reconstruct final private key(s) from partial key(s) info.
 -sp startPubKey: Start the search with a pubKey (for private key splitting)
 -r rekey: Rekey interval in MegaKey, default is disabled
```

Exemple (Windows, GeForce RTX 3060):

```
C:\LTCSearch>LTCVanitySearch.exe -stop -gpu LT
LTCVanitySearch v1.19-LTC
Difficulty: 23
Search: LT [Compressed]
Start Thu Mar  5 21:50:06 2026
Base Key: B1D90C791C84B3BF60FD037656BA758ECCD80E83949B8E27312CD4167AD028BC
Number of CPU thread: 15
GPU: GPU #0 NVIDIA GeForce RTX 3060 (28x128 cores) Grid(224x128)

PubAddress: LT1164Mmmkc8pTiWd1Bjh1CYXYezWmUHf5
Priv (WIF): p2pkh:T91gyr8C9zrnzoNCyQA5dk9sj9m13f9Tms4SP88kwpjKGeNUcsdy
Priv (HEX): 0xB1D90C791C84B3BF60FD037656BA758ECCD80E83949B8E2B312CD4167AD02BAE
```

```
C:\LTCSearch>LTCVanitySearch.exe -stop -gpu MA
LTCVanitySearch v1.19-LTC
Difficulty: 23
Search: MA [Compressed]
Start Thu Mar  5 21:41:58 2026
Base Key: AD8A18E6A58D2951EA2D9EA5FDB02ABDC3185DEDA02AE38E3FF9D208C59772FA
Number of CPU thread: 2

PubAddress: MA11w6618DkHWmxH3VqTcxkoRkZ6hkn5M1
Priv (WIF): p2wpkh-p2sh:T5pGbY7xL88VPN2q3Kf5duKLmN12vG3fN2FcYCdfCbN3CNeZzVfX
Priv (HEX): 0x5275E7195A72D6AE15D2615A024FD540F7967EF90F1DBCAD7FD88C840A9EC067
```

```
C:\LTCSearch>LTCVanitySearch.exe -stop -gpu ltc1qa
LTCVanitySearch v1.19-LTC
Difficulty: 32
Search: ltc1qa [Compressed]
Start Thu Mar  5 21:42:00 2026
Base Key: DF1CC95D1E91390032370C8E05B0C2A1005C3703D445E687AA330C38489D4CAC
Number of CPU thread: 2

PubAddress: ltc1qaqq8p8c8s02hmahnhlw0avkykmmes4l8hs65zv
Priv (WIF): p2wpkh:T7Uq4BtknE7Lxf1EzWPKgFWkerWwaJ1pyjchEHkfzHZemkmBDG6Y
Priv (HEX): 0x84226E6042E7B747F9BA68F80D9391DCD418996748BE1ACFA322A5411B933582
```

# Generate a vanity address for a third party using split-key

It is possible to generate a vanity address for a third party in a safe manner using split-key.\
For instance, Alice wants a nice prefix but does not have CPU power. Bob has the requested CPU power but cannot know the private key of Alice, Alice has to use a split-key.

## Step 1

Alice generates a key pair on her computer then send the generated public key and the wanted prefix to Bob. It can be done by email, nothing is secret.  Nevertheless, Alice has to keep safely the private key and not expose it.
```
LTCVanitySearch.exe -s "AliceSeed" -kp
Priv : T6LXiJv5Pg26vTz7ZRckkZHwxUUHkZYi4167h9Mjc2NGFXpuiafD
Pub  : 0237F8C5CF7543F052C8A1E2242FAE617D0D931EA9B79FB026D7BA4451CA9CF3BE
```
Note: The key pair is a standard SecpK1 key pair and can be generated with a third party software.

## Step 2

Bob runs LTCVanitySearch using the Alice's public key and the wanted prefix.
```
LTCVanitySearch.exe -sp 0237F8C5CF7543F052C8A1E2242FAE617D0D931EA9B79FB026D7BA4451CA9CF3BE -gpu -stop -o keyinfo.txt LAlice
```
It generates a keyinfo.txt file containing the partial private key.
```
PubAddress: LAlice...
PartialPriv: T...
```
Bob sends back this file to Alice. It can also be done by email. The partial private key does not allow anyone to guess the final Alice's private key.

## Step 3

Alice can then reconstructs the final private key using her private key (the one generated in step 1) and the keyinfo.txt from Bob.

```
LTCVanitySearch.exe -rp T6LXiJv5Pg26vTz7ZRckkZHwxUUHkZYi4167h9Mjc2NGFXpuiafD keyinfo.txt

Pub Addr: LAlice...
Priv (WIF): p2pkh:T...
Priv (HEX): 0x...
```

## How it works

Basically the -sp (start public key) adds the specified starting public key (let's call it Q) to the starting keys of each threads. That means that when you search (using -sp), you do not search for addr(k.G) but for addr(k<sub>part</sub>.G+Q) where k is the private key in the first case and k<sub>part</sub> the "partial private key" in the second case. G is the SecpK1 generator point.\
Then the requester can reconstruct the final private key by doing k<sub>part</sub>+k<sub>secret</sub> (mod n) where k<sub>part</sub> is the partial private key found by the searcher and k<sub>secret</sub> is the private key of Q (Q=k<sub>secret</sub>.G). This is the purpose of the -rp option.\
The searcher has found a match for addr(k<sub>part</sub>.G+k<sub>secret</sub>.G) without knowing k<sub>secret</sub> so the requester has the wanted address addr(k<sub>part</sub>.G+Q) and the corresponding private key k<sub>part</sub>+k<sub>secret</sub> (mod n). The searcher is not able to guess this final private key because he doesn't know k<sub>secret</sub> (he knows only Q).

Note: This explanation is simplified, it does not take care of symmetry and endomorphism optimizations but the idea is the same.

# Supported GPU Architectures

| Architecture | SM | Example GPUs |
|-------------|-----|-------------|
| Volta | 7.0, 7.2 | V100, Titan V |
| Turing | 7.5 | RTX 2060/2070/2080/2080Ti, GTX 1660 |
| Ampere | 8.0, 8.6 | A100, RTX 3060/3070/3080/3090 |
| Ada Lovelace | 8.9 | RTX 4060/4070/4080/4090 |
| Blackwell | 12.0 | RTX 5070/5070Ti/5080/5090 |

Forward-compatible PTX is also embedded for future architectures beyond Blackwell.

# Compilation

## Windows

Install CUDA SDK 12.8 and open VanitySearch.sln in Visual Studio 2022.\
You may need to reset your *Windows SDK version* in project properties.\
In Build->Configuration Manager, select the *Release* configuration.\
Build and enjoy.\
\
Note: The current release has been compiled with CUDA SDK 12.8. If you have a different release of the CUDA SDK, you may need to update CUDA SDK paths in VanitySearch.vcxproj using a text editor.

## Linux

 - Install CUDA SDK 12.8+.
 - Install gcc/g++ (>=7).
 - Edit the makefile and set up the appropriate CUDA SDK path if needed. Or pass them as variables to `make` invocation.

    ```make
    CUDA       = /usr/local/cuda
    ```

 - You can enter a list of architectures (refer to nvcc documentation) if you have several GPU with different architecture.

 - Set CCAP to the desired compute capability according to your hardware. Minimum supported is 7.0 (Volta).

 - Go to the LTCVanitySearch directory.
 - To build CPU-only version (without CUDA support):
    ```sh
    $ make all
    ```
 - To build with CUDA:
    ```sh
    $ make gpu=1 CCAP=12.0 all
    ```

# Litecoin Address Format

| Type | Version Byte | Prefix | Example |
|------|-------------|--------|---------|
| P2PKH | 0x30 | L | LT1164Mmmkc8pTiWd1Bjh1CYXYezWmUHf5 |
| P2SH | 0x32 | M | MA11w6618DkHWmxH3VqTcxkoRkZ6hkn5M1 |
| Bech32 | ltc (HRP) | ltc1q | ltc1qaqq8p8c8s02hmahnhlw0avkykmmes4l8hs65zv |
| WIF (compressed) | 0xB0 | T | T91gyr8C9zrnzoNCyQA5dk9sj9m13f9Tms4SP88kwpjKGeNUcsdy |
| WIF (uncompressed) | 0xB0 | 6 | 6... |

# Credits

Based on [VanitySearch](https://github.com/JeanLucPons/VanitySearch) by Jean Luc PONS.

# License

LTCVanitySearch is licensed under GPLv3.
