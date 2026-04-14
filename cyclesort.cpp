int cycleSort(vector<int>& arr){

    int n = arr.size();
    vector<pair<int,int>> indexed(n);
    for(int i = 0; i < n; ++i) indexed[i] = {arr[i], i};
    sort(indexed.begin(), indexed.end());

    int swaps = 0;
    vector<bool> visited(n, false);

    for (int i =0; i < n; ++i){
        if (visited[i] || indexed[i].second == i) continue;

        int j = i, cycleSize = 0;
        while (!visited[j]){
            visited[j] = true;
            j = indexed[j].second;
            ++cycleSize;
        }
        swaps += cycleSize - 1;
    }
    return swaps;
}
