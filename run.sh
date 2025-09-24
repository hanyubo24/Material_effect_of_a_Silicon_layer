#./si_cyl -n 20000 -p e- -pmin 10 -pmax 1500 -o myOutput.csv -z 0.0 -r 300 -th 700

nEvn=30000
save_path=Output/Material_study_30k
for p in e- mu- pi- kaon- proton; do
    for r in 170 240 310 350; do # unit mm
        for th in 200 300 400 500 600 700 750; do   # unit micrometer
            fileName="${save_path}/Silicon_radius_${r}_thickness_${th}_um_particle_${p}.csv"
            echo ./build/si_cyl -n $nEvn -p $p -pmin 10 -pmax 1500 -o $fileName -z 0.0 -r $r -th $th
            ./build/si_cyl -n $nEvn -p $p -pmin 10 -pmax 1500 -o $fileName -z 0.0 -r $r -th $th
        done
    done
done
