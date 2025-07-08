//float4 main() : SV_TARGET
//{
//    return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

struct PixelShederOutput
{
    float4 color : SV_Target0;
};

PixelShederOutput main()
{
    PixelShederOutput output;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return output;
}