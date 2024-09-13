
dataset=t2i-10M
prefix=/root/datasets/${dataset}
save_prefix=/root/indices/${dataset}

cd build

./tests/test_nndescent ${prefix}/base.10M.fbin ${save_prefix}/${dataset}.knng 100 200 24 64 100