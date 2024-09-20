
dataset=laion-10M
prefix=/root/datasets/${dataset}
save_prefix=/root/indices/${dataset}

cd build

./tests/test_nndescent ${prefix}/base.10M.fbin ${save_prefix}/${dataset}.test 100 120 24 150 150