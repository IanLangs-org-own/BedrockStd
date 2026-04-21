for f in *.hpp; do
    echo "==============================="
    echo "$f: "
    echo "-------------------------------"
    echo
    cat "$f"
    echo
    echo "==============================="
done
exit 0 
echo "==============================="
echo "test/test.cpp: "
echo "-------------------------------"
echo
cat "test/test.cpp"
echo
echo "==============================="