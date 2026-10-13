// Huffman_2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Huffman_2.h"
#include <string>
#include <vector>
#include <queue>

#define MAX_LOADSTRING 100
#define IDC_MY_BUTTON 1001 
#define IDC_MY_BUTTON_ADD_NODE 1006 
#define IDC_MY_BUTTON_DELETE_NODE 1007 

#define IDC_MYEDIT  1002
#define IDC_MYEDIT2  1005
#define IDC_LABEL  1003
#define IDC_LABEL2  1004


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int shift_unit = 100;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const int BITMAP_MOVE_DELTA = 2;

const int ID_TIMER = 1;
const int ID_TIMER_x = 2;

const int timer_freq = 20;

bool init_bitmaps_tree = true;

HBITMAP g_hbmMask = NULL;

HBITMAP g_hBitmap = NULL;

struct node_info {
    int dx; // change in x cord for moving the node
	int dy; // change in y cord for moving the node
    int x; // x cord on the screen  
    int y; // y cord on the screen
    int width; // width of the node
	int height; // height of the node
	std::string ch = ""; // character to display
	int freq; // frequency of the character
	int right_child = -1; // index of right child in the node vector
	int left_child = -1; // index of left child in the node vector
    int number_of_leafs = 1;
    bool paint = true;
	std::string code = ""; // code of the character
};

bool sentinel = false;

int  new_node_index = -1;

std::vector<node_info> nodes;

std::vector<std::pair<int, std::vector<int>>> nodes_tree;

std::vector<int> new_node_child;

bool buttonHuffmanPressed = true;

void draw_0_1(HDC hdc, HWND main_window)
{
    
    if (nodes_tree.size() == 1 && nodes.size() > 1) {
		if (nodes[nodes_tree[0].first].paint == false)
			return;

        int root_idx = nodes_tree[0].first;
        std::queue<int> q;
        q.push(root_idx);
        while (!q.empty()) {
            int current_idx = q.front();
            q.pop();
            if (nodes[current_idx].left_child != -1) {
                q.push(nodes[current_idx].left_child);

				RECT rect;
                rect.left = nodes[nodes[current_idx].left_child].dx + nodes[nodes[current_idx].left_child].width;
				rect.top = nodes[nodes[current_idx].left_child].dy;
				rect.right = rect.left + 20; // Width of the text area
				rect.bottom = rect.top + 30; // Height of the text area
                DrawText(
                    hdc,
                    L"0",
                    -1,                                    // -1 means null-terminated string
                    &rect,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER
                );
				nodes[nodes[current_idx].left_child].code = nodes[current_idx].code + "0";
            }
            if (nodes[current_idx].right_child != -1) {
                q.push(nodes[current_idx].right_child);
                RECT rect;
                rect.left = nodes[nodes[current_idx].right_child].dx + nodes[nodes[current_idx].right_child].width;
                rect.top = nodes[nodes[current_idx].right_child].dy;
                rect.right = rect.left + 20; // Width of the text area
                rect.bottom = rect.top + 30; // Height of the text area
                DrawText(
                    hdc,
                    L"1",
                    -1,                                    // -1 means null-terminated string
                    &rect,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER
                );
                nodes[nodes[current_idx].right_child].code = nodes[current_idx].code + "1";
            }
        }
    }
    else {
        return;
    }

    RECT rect;
    RECT rect2;
    RECT rect3;
    if (GetWindowRect(main_window, &rect)) {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        rect2 = { 10, height - 130, 110, height-100 };
        rect3 = {10, height - 170, 110, height-140};
    }

    DrawText(
        hdc,
        L"CODE:",
        -1,                                    // -1 means null-terminated string
        &rect3,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER
    );

    for (int i = 0; i < nodes.size(); i++) {
        if (!nodes[i].ch.empty()) {



            std::string text = nodes[i].ch + ":" + nodes[i].code;
            std::wstring stemp = std::wstring(text.begin(), text.end());
            LPCWSTR sw = stemp.c_str();
            DrawText(
                hdc,
                sw,
                -1,                                    // -1 means null-terminated string
                &rect2,
                DT_SINGLELINE | DT_CENTER | DT_VCENTER
            );
			rect2.left += 100;
			rect2.right += 100;
        }
    }
}

void draw_edges(HDC hdc) 
{
     // 2. Create a custom pen (Solid, 3 pixels wide, Red color)
    HPEN hLinePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    // 3. Select the new pen into the DC and save the old pen
    HPEN hPenOld = (HPEN)SelectObject(hdc, hLinePen);

    // 4. Set the starting point to (X=100, Y=100)
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].paint == false) {
			continue;
        }
        else {
            if (i == 4) {
				int a = 0;

            }
        }
		if (nodes[i].left_child != -1) {
			MoveToEx(hdc, nodes[i].dx + nodes[i].width / 2, nodes[i].dy + nodes[i].height, NULL);
			LineTo(hdc, nodes[nodes[i].left_child].dx + nodes[nodes[i].left_child].width / 2, nodes[nodes[i].left_child].dy);
		}
		if (nodes[i].right_child != -1) {
			MoveToEx(hdc, nodes[i].dx + nodes[i].width / 2, nodes[i].dy + nodes[i].height, NULL);
			LineTo(hdc, nodes[nodes[i].right_child].dx + nodes[nodes[i].right_child].width / 2, nodes[nodes[i].right_child].dy);
		}
	}
    // 6. Clean up: Restore the original pen and delete the custom pen
    SelectObject(hdc, hPenOld);
    DeleteObject(hLinePen);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HUFFMAN2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HUFFMAN2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HUFFMAN2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HUFFMAN2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


HBITMAP bitmap_;
HWND hButton;
HWND hButtonAddNode;
HWND hButtonDeleteAllNodes;
HWND hEdit;
HWND hLabel;
HWND hLabel2;
HWND hEdit2;


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPCHILDREN);

        node_info bm_i_1{5, 10,5, 10,  60, 30, "f", 5 };
        node_info bm_i_2{105, 10,105, 10, 60, 30, "e", 9 };
        node_info bm_i_3{205,10, 205, 10, 60, 30, "c", 12 };
        node_info bm_i_4{305,10,305, 10, 60, 30, "b", 13 };
        node_info bm_i_5{ 405,10,405, 10, 60, 30, "d", 16 };
        node_info bm_i_6{ 505,10,505, 10, 60, 30, "a", 45 };

        nodes.push_back(bm_i_1);
		nodes.push_back(bm_i_2);
		nodes.push_back(bm_i_3);
		nodes.push_back(bm_i_4);
        nodes.push_back(bm_i_5);
        nodes.push_back(bm_i_6);

        std::vector<int> empty_node{ -1 };

        auto p1 = std::make_pair(0, empty_node);
		auto p2 = std::make_pair(1, empty_node);
		auto p3 = std::make_pair(2, empty_node);
		auto p4 = std::make_pair(3, empty_node);
        auto p5 = std::make_pair(4, empty_node);
        auto p6 = std::make_pair(5, empty_node);

        nodes_tree.push_back(p1);
		nodes_tree.push_back(p2);
		nodes_tree.push_back(p3);
		nodes_tree.push_back(p4);
        nodes_tree.push_back(p5);
        nodes_tree.push_back(p6);

        RECT rect;
        int width = 500;
        if (GetClientRect(hWnd, &rect)) {
            width = rect.right; // Since rect.left is 0
            int height = rect.bottom;
        }

        // 2. Calculate the new X position for right alignment
        int buttonX = width - 80 - 10;
        int buttonY = 20; // Fixed top margin (change as needed)

        hButton = CreateWindow(
            L"BUTTON",  // Predefined class name for buttons
            L"HUFFMAN", // Button text label
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            buttonX, 100,     // X and Y position relative to parent
            80, 30,    // Button width and height
            hWnd,       // Handle to the parent window
            (HMENU)IDC_MY_BUTTON, // Button ID (cast to HMENU)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );

        hButtonAddNode = CreateWindow(
            L"BUTTON",  // Predefined class name for buttons
            L"ADD NODE", // Button text label
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            buttonX, 60,     // X and Y position relative to parent
            80, 30,    // Button width and height
            hWnd,       // Handle to the parent window
            (HMENU)IDC_MY_BUTTON_ADD_NODE, // Button ID (cast to HMENU)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );

        hButtonDeleteAllNodes = CreateWindow(
            L"BUTTON",  // Predefined class name for buttons
            L"DELETE", // Button text label
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            buttonX, 140,     // X and Y position relative to parent
            80, 30,    // Button width and height
            hWnd,       // Handle to the parent window
            (HMENU)IDC_MY_BUTTON_DELETE_NODE, // Button ID (cast to HMENU)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );

        hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE,        // Gives it a 3D sunken border
            L"EDIT",                 // Predefined class name for edit controls
            L"",                     // Default text (empty)
            WS_CHILD | WS_VISIBLE | /*WS_VSCROLL | */ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            buttonX -100, 10, 60, 30,        // x, y, width, height
            hWnd,              // Parent window handle
            (HMENU)IDC_MYEDIT,       // Control identifier
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );

        hLabel = CreateWindowW(
            L"static",                    // Predefined class for a label
            L"CHar:",          // The text displayed by the label
            WS_CHILD | WS_VISIBLE | /*SS_LEFT|*/ SS_CENTER | SS_CENTERIMAGE, // Window styles & text alignment
            buttonX - 160, 10,                       // X and Y position coordinates
            40, 30,                      // Width and Height of the control
            hWnd,                         // Handle to the parent window
            (HMENU)IDC_LABEL,                   // Control ID (cast as HMENU)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );

        hLabel2 = CreateWindowW(
            L"static",                    // Predefined class for a label
            L"Freq:",          // The text displayed by the label
            WS_CHILD | WS_VISIBLE | /*SS_LEFT|*/ SS_CENTER | SS_CENTERIMAGE, // Window styles & text alignment
            buttonX - 30, 10,                       // X and Y position coordinates
            40, 30,                      // Width and Height of the control
            hWnd,                         // Handle to the parent window
            (HMENU)IDC_LABEL2,                   // Control ID (cast as HMENU)
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );
        hEdit2 = CreateWindowEx(
            WS_EX_CLIENTEDGE,        // Gives it a 3D sunken border
            L"EDIT",                 // Predefined class name for edit controls
            L"",                     // Default text (empty)
            WS_CHILD | WS_VISIBLE | /*WS_VSCROLL | */ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            buttonX+20, 10, 60, 30,        // x, y, width, height
            hWnd,              // Parent window handle
            (HMENU)IDC_MYEDIT2,       // Control identifier
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );
        HMENU hMenu = GetMenu(hWnd);
        if (hMenu != NULL)
        {
            // Deletes the first menu item (File) completely from the bar
            //UINT menuItemID = GetMenuItemID(hMenu, 0);
            //DeleteMenu(hMenu, menuItemID, MF_BYCOMMAND);
            int itemCount = GetMenuItemCount(hMenu);

            // Loop backwards to safely remove items by their position index
            for (int i = itemCount - 1; i >= 0; i--)
            {
                // MF_BYPOSITION specifies that we are deleting by index, not command ID
                DeleteMenu(hMenu, i, MF_BYPOSITION);
            }


            // Always redraw the menu bar after modification
            DrawMenuBar(hWnd);
        }
        //add textedit for character and frequency of char

        
    }
    break;
    case WM_TIMER:
    {
        RECT rcClient;
        HDC hdc = GetDC(hWnd);

        GetClientRect(hWnd, &rcClient);
        switch (wParam)
        {
        case(ID_TIMER):
            {
            sentinel = false;
                for (int i = 0; i < new_node_child.size(); ++i) {
                    if (new_node_child[i] != -1) {
                        if (nodes[new_node_child[i]].dy < nodes[new_node_child[i]].y) {
                            nodes[new_node_child[i]].dy += BITMAP_MOVE_DELTA;
                        }
                        else {
                            nodes[new_node_child[i]].dy = nodes[new_node_child[i]].y;

                        }
                    }
                    int j = 0;
                    for (0; j < new_node_child.size(); j++) {
                        if (new_node_child[j] != -1) {
                            if (nodes[new_node_child[j]].dy == nodes[new_node_child[j]].y) {
                                continue;
                            }
                            else {
                                j = 0;
                                break;
                            }
                        }
                    }
                    if (j == 0) {

                    }
                    else {
                        nodes[new_node_index].paint = true;
                        KillTimer(hWnd, ID_TIMER);

                        auto ret = SetTimer(hWnd, ID_TIMER_x, timer_freq, NULL);


                    }
                    //bitmaps[new_node_child[i]];
                }
                InvalidateRect(hWnd, NULL, FALSE);
                //UpdateWindow(hWnd);
            }
            break;
        case(ID_TIMER_x): {
            if (init_bitmaps_tree) {
                init_bitmaps_tree = false;
                for (int i = 0; i < nodes_tree.size(); i++) {
                    int freq = nodes[nodes_tree[i].first].freq;
                    if (i + 1 < nodes_tree.size()) {
                        int freq2 = nodes[nodes_tree[i + 1].first].freq;
                        if (freq > freq2) {

                            int num_of_leafs_in_i_node = nodes[nodes_tree[i].first].number_of_leafs;
                            int num_of_leafs_in_i_plus_1_node = nodes[nodes_tree[i + 1].first].number_of_leafs;

                            for (int j = 0; j < nodes_tree[i + 1].second.size(); j++) {
                                if (nodes_tree[i + 1].second[j] != -1) {
                                    nodes[nodes_tree[i + 1].second[j]].x -= shift_unit * num_of_leafs_in_i_node;
                                }
                            }
                            nodes[nodes_tree[i + 1].first].x -= shift_unit * num_of_leafs_in_i_node;

                            for (int j = 0; j < nodes_tree[i].second.size(); j++) {
                                if (nodes_tree[i].second[j] != -1) {
                                    nodes[nodes_tree[i].second[j]].x += shift_unit * num_of_leafs_in_i_plus_1_node;
                                }
                            }

                            nodes[nodes_tree[i].first].x += shift_unit * num_of_leafs_in_i_plus_1_node;

                            std::swap(nodes_tree[i], nodes_tree[i + 1]);
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        break;
                    }

                }
            }
            //KillTimer(hWnd, ID_TIMER_x);
			bool bitmaps_all_dx_x_equal = true;
			for (int i = 0; i < nodes.size(); i++)
			{
                if (nodes[i].dx != nodes[i].x) {
                    bitmaps_all_dx_x_equal = false;
                    if (nodes[i].dx < nodes[i].x) {
                        nodes[i].dx += BITMAP_MOVE_DELTA;
                        if (nodes[i].dx > nodes[i].x) {
                            nodes[i].dx = nodes[i].x;
                        }
                    }
                    else if (nodes[i].dx > nodes[i].x) {
                        nodes[i].dx -= BITMAP_MOVE_DELTA;
                        if (nodes[i].dx < nodes[i].x) {
                            nodes[i].dx = nodes[i].x;
                        }
                    }
                }
                else {
                    continue;
                }
			}

            if (bitmaps_all_dx_x_equal == true) {
                KillTimer(hWnd, ID_TIMER_x);
                init_bitmaps_tree = true;
				buttonHuffmanPressed = true;
            }

            InvalidateRect(hWnd, NULL, FALSE);
            //UpdateWindow(hWnd);
        }
                        break;
        }
        //UpdateBall(&rcClient);
        //drawball2(hdc, bitmaps[0]);

        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                //if (LOWORD(wParam) == IDC_MY_BUTTON)
            case IDC_MY_BUTTON:
                {
                if (buttonHuffmanPressed) {
					buttonHuffmanPressed = false;
                }
                else {
                    break;
                }
                if (nodes_tree.size() < 2) {
                    break;
                }
                //process bitmaps
                auto node1 = nodes_tree[0];
				auto node2 = nodes_tree[1];

				//erase the first two nodes from the tree that will make a new node
                nodes_tree.erase(nodes_tree.begin());
				nodes_tree.erase(nodes_tree.begin());

                //get indexes of a nodes stored in bitmaps vector
				int node_number1 = node1.first;
				int node_number2 = node2.first;

                //creating new node that will be added to the tree
                node_info new_node;
               
				new_node.x = (nodes[node_number1].x + nodes[node_number2].x) / 2;
                new_node.dx = new_node.x;
                new_node.y = 10;
                new_node.dy = 10;
				new_node.width = 60;
				new_node.height = 30;
                new_node.ch = "";
				new_node.freq = nodes[node_number1].freq + nodes[node_number2].freq;
				new_node.number_of_leafs = nodes[node_number1].number_of_leafs + nodes[node_number2].number_of_leafs;
				
                new_node.left_child = node_number1;
                new_node.right_child = node_number2;
                new_node.paint = false;
				
                nodes.push_back(new_node);
                new_node_index = nodes.size() - 1;

				//creating new entry in bitmaps_tree for the new node
                int num_new_bitmap = nodes.size() - 1;
                auto vec_child = node1.second;
				auto vec_child2 = node2.second;
                new_node_child.clear();
				for (int i = 0; i < vec_child.size(); i++) {
					new_node_child.push_back(vec_child[i]);
				}
                for (int i = 0; i < vec_child2.size(); i++) {
					new_node_child.push_back(vec_child2[i]);
                }
				new_node_child.push_back(node_number1);
				new_node_child.push_back(node_number2);
                //bitmaps_tree.
                nodes_tree.insert(nodes_tree.begin(), std::make_pair(num_new_bitmap, new_node_child));
                

                //shift all children of a new node down by shift_unit/2

                for (int i = 0; i < new_node_child.size(); i++) {
					if (new_node_child[i] != -1)
						nodes[new_node_child[i]].y += shift_unit / 2;
                }
                //
                auto ret = SetTimer(hWnd, ID_TIMER, timer_freq, NULL);
                
                InvalidateRect(hWnd, NULL, FALSE);
                
                }
                break;

            case IDC_MY_BUTTON_ADD_NODE:
            {
                wchar_t buffer[10];
                GetWindowText(hEdit, buffer, 10);
                std::wstring ws(buffer);
                std::string char_name(ws.begin(), ws.end());
				if (char_name.empty()) {
					MessageBox(hWnd, L"Character cannot be empty!", L"Error", MB_OK | MB_ICONERROR);
					break;
				}
                wchar_t buffer2[10];
                GetWindowText(hEdit2, buffer2, 10);
                std::wstring ws2(buffer2);       
                std::string freq_value(ws2.begin(), ws2.end());
                if (freq_value.empty()) {
                    MessageBox(hWnd, L"Frequency cannot be empty!", L"Error", MB_OK | MB_ICONERROR);
                    break;
                }

                auto freq_int = std::stoi(freq_value);

                node_info bm_new_node{ 5, 10,5, 10,  60, 30, char_name, freq_int };
                bm_new_node.number_of_leafs = 1;
				bm_new_node.right_child = -1;
				bm_new_node.left_child = -1;

				nodes.push_back(bm_new_node);

				std::vector<int> empty_node{ -1 };

                auto new_tree_node = std::make_pair(nodes.size()-1, empty_node);
                if (nodes_tree.size() > 0) {
                    ;
                }
                int i = 0;
				for (; i < nodes_tree.size(); i++) {
					int freq = nodes[nodes_tree[i].first].freq;
					if (freq < freq_int) {
						//bitmaps[bitmaps_tree[i].first].x += shift_unit;
                        nodes[nodes.size() - 1].x += shift_unit;
                        nodes[nodes.size() - 1].dx += shift_unit;
                        continue;
                    }
                    else {
						for (int j = i; j < nodes_tree.size(); j++) {
                            nodes[nodes_tree[j].first].x += shift_unit;
                            nodes[nodes_tree[j].first].dx += shift_unit;
						}
                        nodes_tree.insert(nodes_tree.begin() + i, new_tree_node);

                        break;
                    }
				}
                if (i == nodes_tree.size()) {
                    nodes_tree.push_back(new_tree_node);
                }
                InvalidateRect(hWnd, NULL, FALSE);
                //UpdateWindow(hWnd);
            }
            break;
            case IDC_MY_BUTTON_DELETE_NODE:
            {
                InvalidateRect(hWnd, NULL, FALSE);
                //UpdateWindow(hWnd);
				nodes.clear();
                nodes_tree.clear();
                buttonHuffmanPressed = true;
                InvalidateRect(hWnd, NULL, FALSE);
                //UpdateWindow(hWnd);
            }
            break;
            case IDM_ABOUT:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_ERASEBKGND:
            return (LRESULT)1;
            break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rc;
            GetClientRect(hWnd, &rc);
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;

            // Create memory DC and a compatible bitmap (The Back Buffer)
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            HBRUSH hBackground = CreateSolidBrush(RGB(255, 255, 255)); // White background
            FillRect(memDC, &rc, hBackground);
            DeleteObject(hBackground);

            //HDC hdcMem = CreateCompatibleDC(hdc);
            HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0)); // 
            HBRUSH hBorderBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red border
            SelectObject(hdc, brush);
            SelectObject(hdc, hBorderBrush);

            int pointSize = 12;
            int lfHeight = -MulDiv(pointSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);

            // 2. Create the new font
            HFONT hNewFont = CreateFont(
                lfHeight,                  // Height of font
                0, 0, 0,                   // Width, escapement, orientation
                FW_NORMAL,                 // Weight (e.g., FW_BOLD)
                FALSE, FALSE, FALSE,       // Italic, Underline, Strikeout
                DEFAULT_CHARSET,           // Charset
                OUT_DEFAULT_PRECIS,        // Output precision
                CLIP_DEFAULT_PRECIS,       // Clipping precision
                DEFAULT_QUALITY,           // Quality
                DEFAULT_PITCH | FF_DONTCARE,// Pitch and family
                L"Arial"                   // Typeface name
            );
            (HFONT)SelectObject(memDC, hNewFont);

            draw_edges(memDC);

			for (int i = 0; i < nodes.size(); i++)
			{
                if (nodes[i].paint) {
                    auto bm_i = nodes[i];
                    RECT rect;
                    rect.left = bm_i.dx;
                    rect.top = bm_i.dy;
                    rect.right = bm_i.dx+bm_i.width;
                    rect.bottom = bm_i.dy+bm_i.height;

                    // 3. Fill the rectangle
                    FillRect(memDC, &rect, brush);
                    FrameRect(memDC, &rect, hBorderBrush);
                    

                    SetTextColor(memDC, RGB(0, 0, 0));
                    std::string text = bm_i.ch + ":" + std::to_string(bm_i.freq);
                    std::wstring stemp = std::wstring(text.begin(), text.end());
                    LPCWSTR sw = stemp.c_str();
                    DrawText(
                        memDC  ,
                        sw,
                        -1,                                    // -1 means null-terminated string
                        &rect,
                        DT_SINGLELINE | DT_CENTER | DT_VCENTER
                    );
                }
			}

			draw_0_1(memDC, hWnd);

            BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            DeleteObject(hNewFont);

            DeleteObject(hBorderBrush);
            // 4. Always clean up GDI objects to avoid memory leaks
            DeleteObject(brush);
            
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
            sentinel = true;
        }
        break;
    case WM_SIZE: {
        // 1. Get the new client width and height of the parent window
        int parentWidth = LOWORD(lParam);
        int parentHeight = HIWORD(lParam);

        // 2. Calculate the new X position for right alignment
        int buttonX = parentWidth - 80 - 10;
        int buttonY = 100; // Fixed top margin (change as needed)

        // 3. Move the button to its new calculated position
        if (hButton != NULL) {
            MoveWindow(hButton, buttonX, buttonY, 80, 30, TRUE);
        }
        if (hButtonAddNode != NULL) {
            MoveWindow(hButtonAddNode, buttonX, buttonY-40, 80, 30, TRUE);
        }
        if (hButtonDeleteAllNodes != NULL) {
            MoveWindow(hButtonDeleteAllNodes, buttonX, buttonY+40, 80, 30, TRUE);
        }

        if (hEdit != NULL) {
            MoveWindow(hEdit, buttonX-100, 10, 60, 30, TRUE);

        }

        if(hLabel != NULL) {
            MoveWindow(hLabel, buttonX - 160, 10, 40, 30, TRUE);
        }

		if (hLabel2 != NULL) {
			MoveWindow(hLabel2, buttonX - 30, 10, 40, 30, TRUE);
		}

		if (hEdit2 != NULL) {
			MoveWindow(hEdit2, buttonX + 20, 10, 60, 30, TRUE);
		}
        
        }
        break;

    case WM_DESTROY:
        //KillTimer(hWnd, ID_TIMER);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
