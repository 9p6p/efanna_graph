//
// Created by 付聪 on 2017/6/21.
//

#include <efanna2e/index_graph.h>
#include <efanna2e/index_random.h>
#include <efanna2e/util.h>

void load_data(char* filename, float*& data, unsigned& num, unsigned& dim) {  // load data with sift10K pattern
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char*)&dim, 4);
    std::cout << "data dimension: " << dim << std::endl;
    in.seekg(0, std::ios::end);
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);
    data = new float[num * dim * sizeof(float)];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);
        in.read((char*)(data + i * dim), dim * 4);
    }
    in.close();
}

template <class T>
T* read_bin(const char* filename, uint32_t& npts, uint32_t& dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    in.read(reinterpret_cast<char*>(&npts), sizeof(uint32_t));
    in.read(reinterpret_cast<char*>(&dim), sizeof(uint32_t));
    std::cout << "Loading data from file: " << filename << ", points_num: " << npts << ", dim: " << dim << std::endl;
    size_t total_size = static_cast<size_t>(npts) * dim;  // notice if the space is exceed the bound size_t max
    if (total_size > std::numeric_limits<size_t>::max() / sizeof(T)) {
        std::cerr << "Requested size is too large." << std::endl;
        exit(EXIT_FAILURE);
    }
    T* data = new T[total_size];
    in.read(reinterpret_cast<char*>(data), total_size * sizeof(T));
    in.close();
    return data;
}

int main(int argc, char** argv) {
    if (argc != 8) {
        std::cout << argv[0] << " data_file save_graph K L iter S R" << std::endl;
        exit(-1);
    }

    // float* data_load = NULL;
    unsigned points_num, dim;
    float* data_load = read_bin<float>(argv[1], points_num, dim);
    // load_data(argv[1], data_load, points_num, dim);

    char* graph_filename = argv[2];
    unsigned K = (unsigned)atoi(argv[3]);
    unsigned L = (unsigned)atoi(argv[4]);
    unsigned iter = (unsigned)atoi(argv[5]);
    unsigned S = (unsigned)atoi(argv[6]);
    unsigned R = (unsigned)atoi(argv[7]);
    // data_load = efanna2e::data_align(data_load, points_num, dim);//one must align the data before build
    efanna2e::IndexRandom init_index(dim, points_num);
    efanna2e::IndexGraph index(dim, points_num, efanna2e::L2, (efanna2e::Index*)(&init_index));

    efanna2e::Parameters paras;
    paras.Set<unsigned>("K", K);
    paras.Set<unsigned>("L", L);
    paras.Set<unsigned>("iter", iter);
    paras.Set<unsigned>("S", S);
    paras.Set<unsigned>("R", R);

    auto s = std::chrono::high_resolution_clock::now();
    index.Build(points_num, data_load, paras);
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = e - s;
    std::cout << "Time cost: " << diff.count() << "\n";

    index.Save(graph_filename);

    return 0;
}
